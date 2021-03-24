#include "lab7-ContainerIfc.h"
#include <sstream>

template <class T>
class MyArray : public ContainerIfc<T> {
   private:
      T *data;
      int size, capacity;   
   public:
      /** Default constructor
      *
      * Default constructor for MyArray
      *
      * Parameters:
      *   none
      *
      * Output:
      *   return: a MyArray with default data members
      *   reference parameters: none
      *   stream: none
      */
      MyArray();
      /** Destructor
      *
      * Deallocates memory taken up by MyArray
      *
      * Parameters:
      *   none
      *
      * Output:
      *   return: none
      *   reference parameters: none
      *   stream: none
      */
      ~MyArray();
      /** Copy Constructor
      *
      * Constructs a new MyArray with the same data as an already
      * constructed MyArray
      *
      * Parameters:
      *   a constant MyArray by reference
      *
      * Output:
      *   return: new copied MyArray
      *   reference parameters: a constant MyArray
      *   stream: none
      */
      MyArray(const MyArray&);
      /** Overloaded assignment operator
      *
      * assigns the data from one MyArray to another MyArray
      *
      * Parameters:
      *   a constant MyArray by reference
      *
      * Output:
      *   return: the new MyArray by reference
      *   reference parameters: a constant MyArray
      *   stream: none
      */
      MyArray<T>& operator=(const MyArray&);
      // Serializable functions
      /** writeObject
      *
      * writes the data of the MyArray object to a binart file
      *
      * Parameters:
      *   an ostream by reference
      *
      * Output:
      *   return: none
      *   reference parameters: an ostream
      *   stream: an output stream
      */
      void writeObject(ostream &);
      /** readObject
      *
      * reads the data of the MyArray object from a binart file
      *
      * Parameters:
      *   an istream by reference
      *
      * Output:
      *   return: none
      *   reference parameters: an istream
      *   stream: an input stream
      */
      void readObject(istream &);
      // Container functions
      /** pushBack
      *
      * adds a value of type T at the end of the array
      *
      * Parameters:
      *   a variable of type T
      *
      * Output:
      *   return: the MyArray by reference
      *   reference parameters: none
      *   stream: none
      */
      MyArray<T>& pushBack(T);
      /** popBack
      *
      * pops the value at the end of the array, and takes it
      * out of the array
      *
      * Parameters:
      *   a variable of type T
      *
      * Output:
      *   return: the MyArray by reference
      *   reference parameters: a variable of type T
      *   stream: none
      */
      MyArray<T>& popBack(T &);
      /** getSize
      *
      * gets the size of the array
      *
      * Parameters:
      *   none
      *
      * Output:
      *   return: an integer, the size of the array
      *   reference parameters: none
      *   stream: none
      */
      int getSize();
      /** isEmpty
      *
      * Checks if the array is empty or not
      *
      * Parameters:
      *   none
      *
      * Output:
      *   return: a boolean value
      *   reference parameters: none
      *   stream: none
      */
      bool isEmpty();
      /** Overloaded access operator
      *
      * Overloaded access operator that access MyArray's
      * datar array
      *
      * Parameters:
      *   an integer, the index to access in the array
      *
      * Output:
      *   return: char by reference
      *   reference parameters: none
      *   stream: none
      */
      T& operator[] (int);
      /** toString
      *
      * Formats the arrays size, capacity, and data in a string so
      * that it can be passed around more easily
      *
      * Parameters:
      *   none
      *
      * Output:
      *   return: a string
      *   reference parameters: none
      *   stream: uses an osstringstream to more easily format the data
      *        and then push it onto a string
      */
      string toString();
};

template <class T>
MyArray<T>::MyArray() {
   this->size = 0;
   this->capacity = 5;
   this->data = new T[this->capacity];
}

template <class T>
MyArray<T>::~MyArray(){
   delete [] this->data;
   this->data = nullptr;
}

template <class T>
MyArray<T>::MyArray(const MyArray& rhs) {
   this->capacity = rhs.capacity;
   this->size = rhs.size;
   this->data = new T[this->capacity];
   for (int i = 0; i < this->size; i++) {
      this->data[i] = rhs.data[i];
   }
}

template <class T>
MyArray<T>& MyArray<T>::operator=(const MyArray& rhs) {
   if (this != &rhs) {
      
      delete [] this->data;
      this->size = rhs.size;
      this->capacity = rhs.capacity;
      this->data = new T[this->capacity];
      
      for (int i = 0; i < this->size; i++) {
         this->data[i] = rhs.data[i];
      }
   }
   return *this;
}

// Serializable
template <class T>
void MyArray<T>::writeObject(ostream &os) {
   os.write(reinterpret_cast<char*>(&this->size), 4);
   os.write(reinterpret_cast<char*>(&this->capacity), 4);
   os.write(reinterpret_cast<char*>(this->data), (sizeof(T) * this->size));   
   return;
}

template <class T>
void MyArray<T>::readObject(istream &is) {
   is.read(reinterpret_cast<char*>(&this->size), sizeof(int));
   is.read(reinterpret_cast<char*>(&this->capacity), sizeof(int));
   if (this->capacity > 5) {
      delete [] this->data;
      this->data = new T[this->capacity];
   }
   is.read(reinterpret_cast<char*>(&this->data), sizeof(T) * this->size); 
   return;  
}


template <class T>
MyArray<T>& MyArray<T>::pushBack(T item) {
   if (this->size == this->capacity) {
      this->capacity += 5;
      T *tmp = new T[this->capacity];
      
      for (int i = 0; i < this->size; i++) {
         tmp[i] = this->data[i];
      }
      
      delete [] this->data;
      this->data = tmp;
   }
   
   this->data[this->size] = item;
   this->size++;
   
   return *this;
}

template <class T>
MyArray<T>& MyArray<T>::popBack(T &item) {
   if (this->size != 0) {
      item = this->data[this->size - 1];
      this->size--;
      if ((this->size < (this->capacity / 4)) && this->capacity > 5) {
         this->capacity -= 5;
         T *tmp = new T[this->capacity];
         
         for (int i = 0; i < this->size; i++) {
            tmp[i] = this->data[i];
         }
         
         delete [] this->data;
         this->data = tmp;
      }
   } else {
      throw BadIndex();
   }
   return *this;
}

template <class T>
int MyArray<T>::getSize() {
   return this->size;
}

template <class T>
bool MyArray<T>::isEmpty() {
   bool result;
   if (this->size == 0) {
      result = true;
   } else {
      result = false;
   }
   
   return result;
}

template <class T>
T& MyArray<T>::operator[](int ndx) {
   if (ndx < 0 || ndx > this->size) {
      throw BadIndex();
   }
   return this->data[ndx];
   
}

template <class T>
string MyArray<T>::toString() {
   ostringstream out;
   out << this->size << endl;
   out << this->capacity << endl;
   for (int i = 0; i < this->size; i++) {
      out << this->data[i] << '\t';
   }
   out << endl;
   return out.str();
}
   
