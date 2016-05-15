Design Ideas: Signals                       {#designsignals}
===========================

A can of worms...
-----------------

Supporting Signals and Slots looked to be an easy implementation until reality dawned upon a pain filled world of complex pitfalls. To see why consider the basic idea of a class (a signal) that calls a list of callbacks when required.  With C++11 bind and lambdas this is fairly straight forward. Nothing like the ease of doing it in Javascript but achievable.

Before we start though, imagine for a second a signal between two sprites. A mouse click on a sprite causes some change we would like to propagate to another sprite. Easy as! just a callback eh.

What happens if the second sprite has been deleted....?  

How does the first sprite know?

OK lets say we keep a list of all signals that are being sent to an object so that when we delete the object we can tell the signals not try to talk to an object thats not there.

What happens if the first sprite was deleted prior to this....?

To handle just this little piece of the problem we need the following.

Requirements
------------
- The source of a signal and the target of a signal must be aware of the other.
- If the target of a signal no longer exists the source must no longer attempt to send to it.
  + *This is equivalent to requiring that if the target of a signal is removed than it must communicate this to the source.*
- If the source of a signal no longer exists the target must no longer attempt to communicate with it.
  + *This is equivalent to requiring that if the source of a signal is removed than it must communicate this to the target.*
  
Solution
--------
After trying various attempts it became clear that we need the following as a minimum.

1. A Signal Class (CSignal) That generates signals as well as maintains and manages a list of slots
that the signal calls.
2. The Signal class must inform any connected slot that it is being removed.  
3. The Signal class must not attempt to communicate with a deleted slot.
4. A Slot Class (CSlot) that provides the binding between the between the signal and the actual function being called. 
5. The Slot class must inform the connected signal that it is being deleted. 
6. The Slot class must not attempt to communicate with a deleted signal. 
7. It is up to the object holding the signal/slot to remove the signal/slot if is no longer required.

There are other features of the solution - but they are not essential to the basic requirements above.

Threading
---------

Thread Safety:  Threading is enabled by default.

TODO:Discuss locking. Whats locked whats not...

Problems
---------

Before you get all excited consider the following discussion - threading is inherently nasty even with mutexes.
	
Imagine a game in which we have a collection of sprites communicating using signals. Give the sprites a rule in which if the health of a sprite drops below a certain amount a signal is fired off to other sprites to ask them to do something. One of the other sprites fires off a heal the other a health buff. 

The sprites are distinct objects so we think think we will be clever and use different threads.

We have two scenarios. In one the Heal gets there first and then the Buff arrives. In the other case the Buff gets there first and then the Heal.

In the first we have 
<CENTER>
\f$ BaseHealth := ( BaseHealth + Heal ) * HealthBoost \f$
</CENTER>

In the second we have 
<CENTER> 
\f$ BaseHealth := BaseHealth*HealthBoost + Heal\f$  
</CENTER>

The trouble is that these are not equal 
<CENTER> 
\f$ (BaseHealth+Heal)*HealthBoost \neq BaseHealth*HealthBoost+Heal \f$
</CENTER> 

In my particular scenario where this library exists as a larger component in an audio processing library it would mean
that a volume effect would produce different outputs depending on thread timing.
<CENTER> 
 \f$(BaseVol+VolChange)*Amplify \neq VolChange+Amplify*BaseVol \f$
</CENTER> 
Note that none of this had anything to do with Mutex locks. It is due to the fact that in general state changes to objects are non abelian. In other words in general unlike familiar things like numbers state changes don't swap \f$ A*B \neq B*A \f$

Mathematically it is the same as \f$ f1(f2(x)) \neq f2(f1(x)) \f$  which in the above translates to  \f$(A+C)*B \neq  A*B+C\f$  

**Does that mean I can't multi thread?**

No it means I am not going to state that signals are thread safe - mutexes or not. I have built in support to enable signals and slots to be accessed from different threads. How you design around it is up to you.

It is your responsibility to handle deleting objects that contain either signals or slots. Internally signals never delete slots and slots never delete signals. They just drop their internal references. Internally I am not using smart pointers which means I am not incrementing any reference counts if you decide to use them. When a slot or a signal is deleted it will inform its counterpart.


Design Features That May Impact Threading.
-----------------------------------------

Both signals and slots maintain references (pointers in fact) to the other.  A Signal maintains a list of slots it is talking to. A slot maintains a pointer back to the signal it is listening for.  When a signal is deleted - most likely because the object containing the signal - such as a sprite - is deleted then all slots on that signal drop their reference back to the signal.

Similiarly when a slot (or the object containing it) is deleted, the slot tells the connected signal to remove its
reference to that slot so that it won't attempt to talk to it.

In any situation where the causal flow of your application results in simultaneous activities down different paths that produce different results that will combine at some point on a single object then the non abelian nature of state change IS going to bite you mutex's or not..
		
Consider other operations outside signalling. Deleting objects containing a signal or slot. Passing objects by value.
Are you doing it? Why are you doing it? What is 'const &' for? Do you know what would happen when the scope of the  function ends and the now copied object phones home to its counterpart to say goodbye...?  Just don't do it.

Restrict your threads for tasks that can clearly be independently broken out. Don't try and be clever it's just too hard.
