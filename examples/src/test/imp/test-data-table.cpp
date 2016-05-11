#pragma once

#include <vector>
#include <string>

namespace evil {

template<typename T>
CDataTable<T>::CDataTable(int cols) :miCols(cols) {
	miColIndex = 0;
	miRowIndex = 0;
};

template<typename T>	
int CDataTable<T>::Cols() { return miCols; }
	
template<typename T>
int CDataTable<T>::Rows() { return (int)mTable.size(); }

	
		///inserts a new row of T
template<typename T>
void CDataTable<T>::insertRow(){
	display_row r(miCols);
	mTable.push_back(r);
}


template<typename T>
int CDataTable<T>::insertCell(T s) {
	if (miColIndex < miCols) {
		mTable[miRowIndex][miColIndex] = s;
	}

	return nextCol();
};

		
template<typename T>
int CDataTable<T>::nextCol() {
	miColIndex++;
	if (miColIndex >= miCols) {
		miColIndex = 0;
	}
	return miColIndex;
}


template<typename T>
int CDataTable<T>::nextRow() {
	if (miRowIndex < mTable.size()) {
		miRowIndex++;
		miColIndex = 0;
	}
	return miRowIndex;
}

template<typename T>
void CDataTable<T>::newRow() {
	insertRow();		
}

template<typename T>
void CDataTable<T>::reset() {
	miColIndex = 0;
	miRowIndex = 0;
}

template<typename T>
T CDataTable<T>::get(int row, int col) {
	//do we need to check bounds??
	return mTable[row][col];
}

template<typename T>
void CDataTable<T>::set(int row, int col, T s) {
	mTable[row][col] = s;
}

template<typename T>
void CDataTable<T>::clear() {
	mTable.clear();
	reset();
}


}