/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __VALUE_OBJECT_H
#define __VALUE_OBJECT_H

#include "Helpers/ConfirmAllowed.h"
#include "Helpers/RemoveConst.h"
#include "Object.h"

namespace ExitGames
{
	namespace Common
	{
		class DictionaryBase;
		
		/**
		   Container class template for objects to be stored as values in a Hashtable or Dictionary.
		   @remarks
		   In most cases the library will do the work of storing a value in a ValueObject for you, so for example you don't have to
		   explicitly create an instance of this class, when storing a key-value pair in a Dictionary or Hashtable instance. However there
		   are some situations, where you will receive instances of class Object or want to create them (for example Hashtable::getValue()
		   will return an Object) and in that case casting those instances into ValueObject-instances can be a convenient way of assuring
		   a type-safe access to their payloads. */
		template<typename Etype>
		class ValueObject : public Object
		{
		public:
			ValueObject(const ValueObject<Etype>& toCopy);
			ValueObject(const Object& obj);
			ValueObject(const Object* const obj);
			ValueObject(typename Helpers::ConfirmAllowed<Etype>::type data);
			ValueObject(typename Helpers::ConfirmAllowed<Etype>::type const pData, short size);
			ValueObject(typename Helpers::ConfirmAllowed<Etype>::type const pData, const short* const sizes);
			virtual ~ValueObject(void);

			virtual ValueObject<Etype>& operator=(const ValueObject<Etype>& toCopy);
			virtual ValueObject<Etype>& operator=(const Object& toCopy);

			Etype getDataCopy(void) const;
			Etype* getDataAddress(void) const;
		protected:
			virtual ValueObject<Etype>& assign(const Object& toCopy);
		private:
			typedef Object super;

			template<typename Ftype> struct getDataCopyImplementation {Ftype operator()(const void* const pData, const short* const pSizes, nByte type, unsigned int dimensions) const;};
			template<typename Ftype> struct getDataCopyImplementation<Ftype*> {Ftype* operator()(const void* const pData, const short* const pSizes, nByte type, unsigned int dimensions) const; private: void* dimensionRecursion(void* pArrayOut, const void* const pArrayIn, const short* const pSizes, nByte type, unsigned int dimensions, unsigned int recursionDepth) const; void* lastDimension(const void* const ArrayIn, short size, nByte type) const;};
			void convert(const Object* const obj, nByte type, unsigned int dimensions);
		};



		/** @file */

		/**
		   Copy-Constructor.

		   Creates an object out of a deep copy of its parameter.

		   The parameter has to be of the same template overload as the
		   object, you want to create.
		   @param toCopy The object to copy.                                */
		template<typename Etype>
		ValueObject<Etype>::ValueObject(const ValueObject<Etype> &toCopy) : Object(toCopy)
		{
		}

		/**
		   Constructor.

		   Creates an object out of a deep copy of the passed Object&.

		   If the type of the content of the passed object does not
		   match the template overload of the object to create, an empty
		   object is created instead of a copy of the passed object,
		   which leads to getDataCopy()
		   and getDataAddress()
		   returning 0.
		   @param obj The Object& to copy.                                 */
		template<typename Etype>
		ValueObject<Etype>::ValueObject(const Object& obj)
		{
			convert(&obj, Helpers::ConfirmAllowed<Etype>::typeName, Helpers::ConfirmAllowed<Etype>::dimensions);
		}

		/**
		   Constructor.

		   Creates an object out of a deep copy of the passed Object*.

		   If the type of the content of the passed object does not
		   match the template overload of the object to create, an empty
		   object is created instead of a copy of the passed object,
		   which leads to getDataCopy()
		   and getDataAddress()
		   return 0.
		   @param obj The Object* to copy.                                 */
		template<typename Etype>
		ValueObject<Etype>::ValueObject(const Object* const obj)
		{
			convert(obj, Helpers::ConfirmAllowed<Etype>::typeName, Helpers::ConfirmAllowed<Etype>::dimensions);
		}

		/**
		   Constructor.
	   
		   Creates an object out of a deep copy of the passed single-value Etype.
	   
		   @param data The value to copy. Has to be of a supported type.                         */
		template<typename Etype>
		ValueObject<Etype>::ValueObject(typename Helpers::ConfirmAllowed<Etype>::type data) : Object(&data, Helpers::ConfirmAllowed<Etype>::typeName, Helpers::ConfirmAllowed<Etype>::customTypeName, true)
		{
			COMPILE_TIME_ASSERT2_TRUE_MSG(!Helpers::ConfirmAllowed<Etype>::dimensions, ERROR_THIS_OVERLOAD_IS_ONLY_FOR_SINGLE_VALUES);
		}

		/**
		   Constructor.
	   
		   Creates an object out of a deep copy of the passed single-dimensional Etype-array.
	   
		   @param pData The array to copy.
		   @param size The element count of data.                               */
		template<typename Etype>
		ValueObject<Etype>::ValueObject(typename Helpers::ConfirmAllowed<Etype>::type const pData, short size) : Object(const_cast<typename Helpers::RemoveConst<Etype>::type>(pData), Helpers::ConfirmAllowed<Etype>::typeName, Helpers::ConfirmAllowed<Etype>::customTypeName, size, true)
		{
			COMPILE_TIME_ASSERT2_TRUE_MSG(Helpers::ConfirmAllowed<Etype>::dimensions==1, ERROR_THIS_OVERLOAD_IS_ONLY_FOR_1D_ARRAYS);
		}

		/**
		   Constructor.
	   
		   Creates an object out of a deep copy of the passed multi-dimensional Etype-array.
	   
		   @param pData The array to copy.
		   @param sizes The array of element counts for the different dimensions of data.                               */
		template<typename Etype>
		ValueObject<Etype>::ValueObject(typename Helpers::ConfirmAllowed<Etype>::type const pData, const short* const sizes) : Object(const_cast<typename Helpers::RemoveConst<Etype>::type>(pData), Helpers::ConfirmAllowed<Etype>::typeName, Helpers::ConfirmAllowed<Etype>::customTypeName, Helpers::ConfirmAllowed<Etype>::dimensions, sizes, true)
		{
			COMPILE_TIME_ASSERT2_TRUE_MSG((bool)Helpers::ConfirmAllowed<Etype>::dimensions, ERROR_THIS_OVERLOAD_IS_ONLY_FOR_FOR_ARRAYS);
		}

		/**
		   Destructor.*/
		template<typename Etype>
		ValueObject<Etype>::~ValueObject(void)
		{
		}

		/**
		   operator = : Makes a deep copy of its right operand into its
		   left operand. This overwrites old data in the left operand.  */
		template<typename Etype>
		ValueObject<Etype>& ValueObject<Etype>::operator=(const ValueObject<Etype>& toCopy)
		{
			return assign(toCopy);
		}

		/**
		   operator = : Makes a deep copy of its right operand into its
		   left operand. This overwrites old data in the left operand.

		   If the type of the content of the right operand does not
		   match the template overload of the left operand, then the left
		   operand stays unchanged.  */
		template<typename Etype>
		ValueObject<Etype>& ValueObject<Etype>::operator=(const Object& toCopy)
		{
			return assign(toCopy);
		}

		template<typename Etype>
		ValueObject<Etype>& ValueObject<Etype>::assign(const Object& toCopy)
		{
			if(Helpers::ConfirmAllowed<Etype>::typeName == toCopy.getType())
				super::assign(toCopy);
			return *this;
		}

		/**
		   Returns a deep copy of the content of the object. If you
		   only need access to the content, while the object still
		   exists, you can use getDataAddress()
		   instead to avoid the deep copy. That is especially
		   interesting for large content, of course.

		   If successful, the template overloads for array types of this
		   function allocate the data for the copy, so you have to free
		   (for arrays of primitive types) or delete (for arrays of
		   class objects) it, as soon, as you do not need the array
		   anymore. All non-array copies free there memory
		   automatically, as soon as they leave their scope, same as the
		   single indices of the array, as soon, as the array is freed.

		   In case of an error this function returns 0 for primitive
		   return types and for arrays and empty objects for classes.
		   @returns
		   a deep copy of the content of the object if successful, 0 or
		   an empty object otherwise.                                               */

		template<typename Etype>
		Etype ValueObject<Etype>::getDataCopy(void) const
		{
			return getDataCopyImplementation<Etype>()(getData(), getSizes(), getType(), getDimensions());
		}

		template<typename Etype>
		template<typename Ftype>
		Ftype ValueObject<Etype>::getDataCopyImplementation<Ftype>::operator()(const void* const pData, const short* const /*pSizes*/, nByte type, unsigned int dimensions) const
		{
			return (type == Helpers::ConfirmAllowed<Ftype>::typeName && !dimensions)?*(Ftype*)pData:Ftype();
		}

		template<typename Etype>
		template<typename Ftype>
		Ftype* ValueObject<Etype>::getDataCopyImplementation<Ftype*>::operator()(const void* const pData, const short* const pSizes, nByte type, unsigned int dimensions) const
		{
			if(type == Helpers::ConfirmAllowed<Ftype*>::typeName && dimensions)
				return reinterpret_cast<Ftype*>(dimensionRecursion(NULL, pData, pSizes, type, dimensions, 0));
			else
				return NULL;
		}

		template<typename Etype>
		template<typename Ftype>
		void* ValueObject<Etype>::getDataCopyImplementation<Ftype*>::dimensionRecursion(void* pArrayOut, const void* const pArrayIn, const short* const pSizes, nByte type, unsigned int dimensions, unsigned int recursionDepth) const
		{
			if(dimensions && recursionDepth < dimensions-1)
			{
				int i;
				pArrayOut = MALLOC(sizeof(void*)*pSizes[recursionDepth]);
				for(i=0; i<pSizes[recursionDepth]; ++i)
					((void**)pArrayOut)[i] = dimensionRecursion(((void**)pArrayOut)[i], ((void**)pArrayIn)[i], pSizes, type, dimensions, recursionDepth+1);
				return pArrayOut;
			}
			else
				return pArrayOut = lastDimension(pArrayIn, pSizes[recursionDepth], type);
		}

		template<typename Etype>
		template<typename Ftype>
		void* ValueObject<Etype>::getDataCopyImplementation<Ftype*>::lastDimension(const void* const pArrayIn, short size, nByte type) const
		{
			// Helpers::ConfirmAllowed<Ftype*>::scalarType* will be a 1d pointer, even if Ftype* has multiple pointer abstraction levels
			typename Helpers::ConfirmAllowed<Ftype*>::scalarType* pRetVal = new typename Helpers::ConfirmAllowed<Ftype*>::scalarType[size];
			for(short i=0; i<size; i++)
				pRetVal[i] = (((typename Helpers::ConfirmAllowed<Ftype*>::scalarType*)(pArrayIn))[i]);
			return pRetVal;
		}

		/**
		   Returns the address of the original content of the object.
		   If you need access to the data above lifetime of the object,
		   call getDataCopy().

		   The return type is a pointer to the data, so it is a
		   double-pointer, of course, for template overloads, which data
		   already is a pointer.

		   In case of an error, this function returns NULL.
		   @returns
		   the address of the original content of the object, if
		   successful, NULL otherwise.                                      */
		template<typename Etype>
		Etype* ValueObject<Etype>::getDataAddress(void) const
		{
			return (getType() == Helpers::ConfirmAllowed<Etype>::typeName && getDimensions() == Helpers::ConfirmAllowed<Etype>::dimensions)?(Etype*)getData():NULL;
		}

		template<typename Etype>
		void ValueObject<Etype>::convert(const Object* const obj, nByte type, unsigned int dimensions)
		{
			super::assign((obj && type == obj->getType()  && dimensions == obj->getDimensions())?*obj:Object());
		}
	}
}

#endif