/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is Netscape Communications
 * Corporation.  Portions created by Netscape are Copyright (C) 1998
 * Netscape Communications Corporation.  All Rights Reserved.
 */

/*

  DO NOT USE THIS. IT IS INTENDED FOR TEMPORARY USE BY GLOBAL HISTORY,
  PENDING CONVERSION OF THE MDB INTERFACES TO XPCOM.

*/


#ifndef nsMdbPtr_h__
#define nsMdbPtr_h__

#include "mdb.h"

template <class T>
class nsMdbDerivedSafe : public T
{
private:
    virtual mdb_err AddStrongRef(nsIMdbEnv* aEnv);    // NOT TO BE IMPLEMENTED
    virtual mdb_err CutStrongRef(nsIMdbEnv* aEnv);    // NOT TO BE IMPLEMENTED
    virtual nsMdbDerivedSafe<T>& operator=(const T&); // NOT TO BE IMPLEMENTED
    void operator delete(void*, size_t);              // NOT TO BE IMPLEMENTED
};


template <class T>
class nsMdbPtr
{
private:
    nsIMdbEnv* mEnv;
    T* mRawPtr;

public:
    nsMdbPtr(nsIMdbEnv* aEnv) : mEnv(aEnv), mRawPtr(0)
    {
        NS_PRECONDITION(aEnv != 0, "null ptr");
    }

    nsMdbPtr(nsIMdbEnv* aEnv, T* aRawPtr) : mEnv(aEnv), mRawPtr(0)
    {
        NS_PRECONDITION(aEnv != 0, "null ptr");
        if (mEnv) {
            if (aRawPtr) {
                mRawPtr = aRawPtr;
                mRawPtr->AddStrongRef(mEnv);
            }
        }
    }

    nsMdbPtr(const nsMdbPtr<T>& aSmartPtr) : mEnv(aSmartPtr.mEnv), mRawPtr(0)
    {
        if (mEnv) {
            if (aRawPtr) {
                mRawPtr = aRawPtr;
                mRawPtr->AddStrongRef(mEnv);
            }
        }
    }

    nsMdbPtr<T>&
    operator=(const nsMdbPtr<T>& aSmartPtr)
    {
        if (mEnv) {
            if (mRawPtr) {
                mRawPtr->CutStrongRef(mEnv);
                mRawPtr = 0;
            }
        }
        mEnv = aSmartPtr.mEnv;
        if (mEnv) {
            mRawPtr = aSmartPtr.mRawPtr;
            if (mRawPtr)
                mRawPtr->AddStrongRef(mEnv);
        }
    }

    ~nsMdbPtr()
    {
        if (mEnv) {
            if (mRawPtr)
                mRawPtr->CutStrongRef(mEnv);
        }
    }

    nsMdbDerivedSafe<T>*
    get() const
    {
        return NS_REINTERPRET_CAST(nsMdbDerivedSafe<T>*, mRawPtr);
    }

    nsMdbDerivedSafe<T>*
    operator->() const
    {
        return get();
    }


    nsMdbDerivedSafe<T>&
    operator*() const
    {
        return *get();
    }

    operator nsMdbDerivedSafe<T>*() const
    {
        return get();
    }


    T**
    StartAssignment()
    {
        if (mRawPtr) {
            mRawPtr->CutStrongRef(mEnv);
            mRawPtr = 0;
        }
        return &mRawPtr;
    }
};

template <class T, class U>
inline
PRBool
operator==(const nsMdbPtr<T>& lhs, const nsMdbPtr<U>& rhs)
{
    return NS_STATIC_CAST(const void*, lhs.get()) == NS_STATIC_CAST(const void*, rhs.get());
}

template <class T, class U>
inline
PRBool
operator==(const nsMdbPtr<T>& lhs, const U* rhs)
{
    return NS_STATIC_CAST(const void*, lhs.get()) == NS_STATIC_CAST(const void*, rhs);
}


template <class T, class U>
inline
PRBool
operator==(const U* lhs, const nsMdbPtr<T>& rhs)
{
    return NS_STATIC_CAST(const void*, lhs) == NS_STATIC_CAST(const void*, rhs.get());
}




template <class T, class U>
inline
PRBool
operator!=(const nsMdbPtr<T>& lhs, const nsMdbPtr<U>& rhs)
{
    return NS_STATIC_CAST(const void*, lhs.get()) != NS_STATIC_CAST(const void*, rhs.get());
}

template <class T, class U>
inline
PRBool
operator!=(const nsMdbPtr<T>& lhs, const U* rhs)
{
    return NS_STATIC_CAST(const void*, lhs.get()) != NS_STATIC_CAST(const void*, rhs);
}


template <class T, class U>
inline
PRBool
operator!=(const U* lhs, const nsMdbPtr<T>& rhs)
{
    return NS_STATIC_CAST(const void*, lhs) != NS_STATIC_CAST(const void*, rhs.get());
}



template <class T>
class nsGetterAcquires
{
private:
    nsMdbPtr<T>& mTargetSmartPtr;

public:
    explicit
    nsGetterAcquires(nsMdbPtr<T>& aSmartPtr) : mTargetSmartPtr(aSmartPtr)
    {
        // nothing else to do
    }

    operator T**()
    {
        return mTargetSmartPtr.StartAssignment();
    }        
};


template <class T>
inline
nsGetterAcquires<T>
getter_Acquires(nsMdbPtr<T>& aSmartPtr)
{
    return nsGetterAcquires<T>(aSmartPtr);
}


#endif // nsMdbPtr_h__

