#ifndef CLASSTEMPLATE_MATRIX_H
#define CLASSTEMPLATE_MATRIX_H

#define MAXROWS 5
#define MAXCOLS 5

template<class T>
class Matrix
{
private:
    T matrix[MAXROWS][MAXCOLS];
    int rows;
    int cols;

public:
    // constructor Initialize all the values of matrix to zero
    Matrix();  // Set rows to MAXROWS and cols to MAXCOLS
    Matrix(int col, int row);
    //print Function
    void printMatrix();

    // Setter Functions
    void setMatrix(T[][MAXCOLS]);    //set the array to what is sent
    void addMatrix(T[][MAXCOLS]);   //add an array to matrix

    // No destructor needed
};

#endif //CLASSTEMPLATE_MATRIX_H

template<class T>
Matrix<T>::Matrix()
{
    rows = MAXROWS;
    cols = MAXCOLS;
}

template<class T>
Matrix<T>::Matrix(int row,int col)
{
    rows = row;
    cols = col;
}

template<class T>
void Matrix<T>::setMatrix(T array[][MAXCOLS])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            matrix[i][j] = array[i][j];
    }
}

template<class T>
void Matrix<T>::printMatrix()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            std::cout << matrix[i][j] << "  ";

        std::cout << std::endl;
    }
}

template<class T>
void Matrix<T>::addMatrix(T otherArray[][MAXCOLS])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            matrix[i][j] += otherArray[i][j];
    }
}
