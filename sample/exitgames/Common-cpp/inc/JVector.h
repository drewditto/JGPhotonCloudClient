/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __JVECTOR_H
#define __JVECTOR_H

#include "Base.h"
#include "Helpers/IsPrimitiveType.h"
#include "Helpers/ToStringImplementation.h"

#undef min

namespace ExitGames
{
	namespace Common
	{
		/**
		   This is a C++ implementation of the <i>Vector</i> Container
		   class from Sun Java.
		   @details
		   This class is based on the Java Vector class and as such
		   contains all the public member functions of its Java
		   equivalent. Unlike Java, typecasts are not necessary since
		   C++ allows template instantiation of types at compile time.
		   In addition to the Java public member functions, some
		   operators were also added in order to take advantage of the
		   operator overloading feature available in C++.     */
		template<typename Etype>
		class JVector : public Base
		{
		public:
			using Base::toString;

			JVector(unsigned int initialCapacity=40, unsigned int capacityIncrement=10);
			virtual ~JVector(void);

			JVector(const JVector<Etype>& rhv);
			JVector& operator=(const JVector<Etype> &rhv);

			bool operator==(const JVector<Etype>& toCompare) const;
			bool operator!=(const JVector<Etype>& toCompare) const;
			const Etype& operator[](unsigned int index) const;
			Etype& operator[](unsigned int index);

			unsigned int getCapacity(void) const;
			bool contains(const Etype& elem) const;
			const Etype& getFirstElement(void) const;
			int getIndexOf(const Etype& elem) const;
			bool getIsEmpty(void) const;
			const Etype& getLastElement(void) const;
			int getLastIndexOf(const Etype& elem) const;
			unsigned int getSize(void) const;
			void copyInto(Etype* array) const;
			void addElement(const Etype& obj);
			void ensureCapacity(unsigned int minCapacity);
			void removeAllElements(void);
			bool removeElement(const Etype& obj);
			void trimToSize(void);
			Etype& getElementAt(unsigned int index) const;
			void insertElementAt(const Etype &obj, unsigned int index);
			void removeElementAt(unsigned int index);
			void setElementAt(const Etype &obj, unsigned int index);
			JString& toString(JString& retStr, bool withTypes=false) const;
		private:
			int min(unsigned int left, unsigned int right) const;
			void verifyIndex(unsigned int index) const;
			JString primitiveToString(void) const;
			JString objectToString(void) const;

			unsigned int mSize;
			unsigned int mCapacity;
			unsigned int mIncrement;
			Etype** mpData;
		};



		/** @file */

		/**
		   Constructor.
	   
		   Creates an empty JVector of elements of the type of
		   the template parameter. Neutron mostly uses Object
		   as type, to wrap the real datatype, which makes it possible,
		   to put different types of data into the same JVector.
		   @param initialCapacity the amount of elements, the JVector can take without need for resize. If you choose this too small, the JVector needs expensive resizes later (it's most likely, that the complete memory has to be copied to a new location on resize), if you choose it too big, you will waste much memory. The default is 40.
		   @param capacityIncrement  Every time, when one adds an element to the Vector and it has no capacity left anymore, it's capacity will grow with this amount of elements on automatic resize. If you pass a too small value here, expensive resize will be needed more often, if you choose a too big one, possibly memory is wasted. The default is 10.                                         */
		template<typename Etype>
		JVector<Etype>::JVector(unsigned int initialCapacity, unsigned int capacityIncrement) 
		{
			mSize = 0;
			mCapacity = initialCapacity;
			mpData = new Etype*[mCapacity];
			mIncrement = capacityIncrement;
		}
	
		/**
		   Destructor. */
		template<typename Etype>
		JVector<Etype>::~JVector()
		{
			removeAllElements();
			delete[] mpData;
		}

		/**
		   Copy-Constructor.
	   
		   Creates an object out of a deep copy of its parameter.
	   
		   The parameter has to be of the same template overload as the
		   object, you want to create.
		   @param toCopy The object to copy.                                */
		template<typename Etype>
		JVector<Etype>::JVector(const JVector<Etype>& toCopy)
			: mSize(0)
			, mpData(NULL)
		{
			*this = toCopy;
		}

		/**
		   operator =.
	   
		   Makes a deep copy of its right operand into its left operand.
		   Both operands have to be of the same template overload.
	   
		   This overwrites old data in the left operand.                 */
		template<typename Etype>
		JVector<Etype>& JVector<Etype>::operator=(const JVector<Etype> &toCopy)
		{
			if(mpData)
			{
				removeAllElements();
				delete[] mpData;
			}

			mSize = toCopy.mSize;
			mCapacity = toCopy.mCapacity;

			mpData = new Etype*[mCapacity];
			mIncrement = toCopy.mIncrement;

			for(unsigned int i=0; i<mSize; i++)
				mpData[i] = new Etype(*(toCopy.mpData[i]));
			return *this;
		}
		
		/**
		   operator==.
		   @returns true, if both operands are equal, false otherwise.
		   Two instances are treated as equal, if they both contain the the same amount of elements and every element of one instance
		   equals the other instance's element at the same index. If the element type is a pointer type, then the pointers are checked
		   for equality, not the values, to which they point to. */
		template<typename Etype>
		bool JVector<Etype>::operator==(const JVector<Etype>& toCompare) const
		{
			if(getSize() != toCompare.getSize())
				return false;
			for(unsigned int i=0; i<getSize(); ++i)
				if((*this)[i] != toCompare[i])
					return false;
			return true;
		}

		/**
		   operator!=.
		   @returns false, if operator==() would return true, true otherwise. */
		template<typename Etype>
		bool JVector<Etype>::operator!=(const JVector<Etype>& toCompare) const
		{
			return !(*this == toCompare);
		}

		/**
		   operator[]. Wraps the function getElementAt(),
		   so you have same syntax like for arrays.                                                 */
		template<typename Etype>
		const Etype& JVector<Etype>::operator[](unsigned int index) const
		{
			return getElementAt(index);
		}

		/**
		   operator[]. Wraps the function getElementAt(),
		   so you have same syntax like for arrays.                                                 */
		template<typename Etype>
		Etype& JVector<Etype>::operator[](unsigned int index)
		{
			verifyIndex(index);
			return *mpData[index];
		}

		/**
		   Returns the current capacity of the JVector.
		   @returns
		   the current capacity.                                */
		template<typename Etype>
		unsigned int JVector<Etype>::getCapacity(void) const
		{
			return mCapacity;
		}

		/**
		   Checks, if the JVector contains the passed data as an
		   element.
		   @param elem a reference to the data, you want to check. Needs to be either a primitive type or an object of a class with an overloaded == operator.
		   @returns
		   true, if the element was found, false otherwise.             */
		template<typename Etype>
		bool JVector<Etype>::contains(const Etype &elem) const
		{
			for(unsigned int i=0; i<mSize; i++)
			{
				if(*mpData[i] == elem)
					return true;
			}

			return false;
		}

		/**
		   Copies all elements of the JVector into the passed
		   array. The caller has to make sure, the array is big enough,
		   to take all elements of the vector, otherwise calling this
		   function produces a buffer overflow.
		   @param array  an array of variables of the type of the template
					overload.
		   @returns
		   nothing.                                                     */
		template<typename Etype>
		void JVector<Etype>::copyInto(Etype* array) const
		{
			for(unsigned int i=0; i<mSize; i++)
				array[i] = *mpData[i];
		}

		/**
		   Returns the element of the JVector at the passed
		   index. This does not check for valid indexes and shows
		   unexpected behavior for invalid indexes!
		   @param index the index of the element, that should be returned. Must not be bigger than the current size of the vector!
		   @returns
		   the element at the passed index.                            */
		template<typename Etype>
		Etype& JVector<Etype>::getElementAt(unsigned int index) const
		{
			verifyIndex(index);
			return *mpData[index];
		}

		/**
		   Returns the first element of the JVector. Shows
		   unexpected behaviour for empty vectors.
		   @returns
		   the first element.                                      */
		template<typename Etype>
		const Etype& JVector<Etype>::getFirstElement(void) const
		{
			verifyIndex(0);
			return *mpData[0];
		}

		/**
		   Searches the JVector from the first element in forward
		   direction for the passed element and returns the first index,
		   where it was found.
		   @param elem the element, to search for.
		   @returns
		   the index of the first found of the passed element or -1, if
		   the element could not be found at all.                        */
		template<typename Etype>
		int JVector<Etype>::getIndexOf(const Etype &elem) const
		{
			for(unsigned int i=0; i<mSize; i++)
				if(*mpData[i] == elem)
					return i;
			return -1;
		}

		/**
		   Checks, if the JVector is empty.
		   @returns
		   true, if the JVector is empty, or false, if it contains at
		   least one element.                                         */
		template<typename Etype>
		bool JVector<Etype>::getIsEmpty(void) const
		{
			return mSize == 0;
		}

		/**
		   Returns the last element of the JVector. Shows
		   unexpected behaviour for empty vectors.
		   @returns
		   the last element.                                      */
		template<typename Etype>
		const Etype& JVector<Etype>::getLastElement() const
		{
			verifyIndex(0);
			return *mpData[mSize - 1];
		}

		/**
		   Searches the JVector from the last element in backward
		   direction for the passed element and returns the first index,
		   where it was found.
		   @param elem the element, to search for.
		   @returns
		   the index of the first found of the passed element or -1, if
		   the element could not be found at all.                        */
		template<typename Etype>
		int JVector<Etype>::getLastIndexOf(const Etype &elem) const
		{
			for(unsigned int i=mSize; i; i--)
				if(*mpData[i] == elem)
					return i;
			return -1;
		}

		/**
		   Returns the size of the JVector.
		   @returns
		   the size.                              */
		template<typename Etype>
		unsigned int JVector<Etype>::getSize(void) const
		{
			return mSize;
		}

		/**
		   Adds an element to the JVector. This automatically
		   resizes the JVector to it's old size + the capacityIncrement,
		   you passed, when creating the vector (if you passed no value for
		   capacityIncrement, then it was set to it's default value (see
		   constructor doc)), if the size of the JVector has already
		   reached it's capacity, which means, that most likely the
		   whole vector has to be copied to new memory. So this can be
		   an expensive operation for huge vectors.
		   @param elem the element, to add.
		   @returns
		   nothing.                                                      */
		template<typename Etype>
		void JVector<Etype>::addElement(const Etype &elem)
		{
			if(mSize == mCapacity)
				ensureCapacity(mCapacity+mIncrement);
			mpData[mSize++] = new Etype(elem);
		}

		/**
		   Resizes the JVector to the passed getCapacity, if it's
		   old getCapacity has been smaller. Most likely the whole JVector
		   has to be copied into new memory, so this is an expensive
		   operation for huge JVectorBases. Call this function, before you
		   add a lot of elements to the vector, to avoid multiple
		   expensive resizes through adding.
		   @param minCapacity the new capacity for the JVector.
		   @returns
		   nothing.                                                      */
		template<typename Etype>
		void JVector<Etype>::ensureCapacity(unsigned int minCapacity)
		{
			if(minCapacity > mCapacity)
			{
				unsigned int i;
				mCapacity = minCapacity;
				Etype** temp = new Etype*[mCapacity];

				//copy all the elements over upto newsize
				for(i=0; i<mSize; i++)
					temp[i] = mpData[i];

				delete[] mpData;

				mpData = temp;
			}
		}

		/**
		   Inserts parameter one into the JVector at the index,
		   passed as parameter two. Because all elements above or at the
		   passed index have to be moved one position up, it is
		   expensive, to insert an element at an low index into a huge
		   JVector.
		   @param obj the element, to insert.
		   @param index the position in the JVector, the element is inserted at.
		   @returns
		   nothing.                                                      */
		template<typename Etype>
		void JVector<Etype>::insertElementAt(const Etype &obj, unsigned int index)
		{
			if(index == mSize)
				addElement(obj);
			else
			{
				verifyIndex(index);

				if(mSize == mCapacity)
					ensureCapacity(mCapacity + mIncrement);

				Etype* newItem = new Etype;
				*newItem=obj;

				Etype* tmp; //temp to hold item to be moved over.
				for(unsigned int i=index; i<=mSize; i++)
				{
					tmp = mpData[i];
					mpData[i] = newItem;

					if(i != mSize)
						newItem = tmp;
					else
						break;
				}
				mSize++;
			}
		}

		/**
		   Clears the JVector.
		   @returns
		   nothing.                   */
		template<typename Etype>
		void JVector<Etype>::removeAllElements()
		{
			for(unsigned int i=0; i<mSize; i++)
				delete mpData[i];

			mSize = 0;
		}

		/**
		   Removes the passed element from the JVector.
		   @param obj the element, to remove.
		   @returns
		   true, if the element has been removed, false, if it could not
		   be found.                                                     */
		template<typename Etype>
		bool JVector<Etype>::removeElement(const Etype &obj)
		{
			for(unsigned int i=0; i<mSize; i++)
			{
				if(*mpData[i] == obj)
				{
					removeElementAt(i);
					return true;
				}
			}
			return false;
		}

		/**
		   Removes the element at the passed index from the JVector.
		   Shows unexpected behaviour for invalid indexes.
		   @param index the index of the element to remove.
		   @returns
		   nothing.                                                         */
		template<typename Etype>
		void JVector<Etype>::removeElementAt(unsigned int index)
		{
			verifyIndex(index);

			delete mpData[index];

			for(unsigned int i=index+1; i<mSize; i++)
				mpData[i-1] = mpData[i];

			mSize--;
		}

		/**
		   Sets the element at the passed index of the JVector to
		   the passed new value. Shows unexpected behaviour for invalid
		   indexes.
		   @param obj the new value.
		   @param index the index of the element, which is set to the new value.
		   @returns
		   nothing.                                                      */
		template<typename Etype>
		void JVector<Etype>::setElementAt(const Etype &obj, unsigned int index)
		{
			verifyIndex(index);
		
			*mpData[index] = obj;
		}

		/**
		   Trims the capacity of the JVector to the size, it
		   currently uses. Call this function for a JVector with huge
		   unused capacity, if you do not want to add further elements
		   to it and if you are short of memory. This function copies the
		   whole vector to new memory, so it is expensive for huge
		   vectors. If you only add one element to the JVector later,
		   it's copied again.                                           */
		template<typename Etype>
		void JVector<Etype>::trimToSize(void)
		{
			if(mSize != mCapacity)
			{
				Etype** temp = new Etype*[mSize];
				unsigned int i;

				for(i=0; i<mSize; i++)
					temp[i] = mpData[i];

				delete[] mpData;

				mpData = temp;
				mCapacity = mSize;
			}
		}

		template<typename Etype>
		int JVector<Etype>::min(unsigned int left, unsigned int right) const
		{
			return left<right?left:right;
		}
	  
		template<typename Etype>
		void JVector<Etype>::verifyIndex(unsigned int index) const
		{
			if(index >= mSize)
				debugReturn("JVector: Index Out Of Bounds");
		}

		template<typename Etype>
		JString& JVector<Etype>::toString(JString& retStr, bool /*withTypes*/) const
		{
			return retStr += Helpers::ToStringImplementation<Helpers::IsPrimitiveType<Etype>::is, Etype>::converter.toString(this->mpData, this->getSize());
		}
	}
}

#endif