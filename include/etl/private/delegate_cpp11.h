///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

/******************************************************************************

Copyright (C) 2017 by Sergey A Kryukov: derived work
http://www.SAKryukov.org
http://www.codeproject.com/Members/SAKryukov

Based on original work by Sergey Ryazanov:
"The Impossibly Fast C++ Delegates", 18 Jul 2005
https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

MIT license:
http://en.wikipedia.org/wiki/MIT_License

Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

******************************************************************************/

#ifndef ETL_DELEGATE_CPP11_INCLUDED
#define ETL_DELEGATE_CPP11_INCLUDED

#include "../platform.h"
#include "../error_handler.h"
#include "../exception.h"
#include "../type_traits.h"
#include "../function_traits.h"
#include "../utility.h"
#include "../optional.h"

namespace etl
{
  //***************************************************************************
  /// The base class for delegate exceptions.
  //***************************************************************************
  class delegate_exception : public exception
  {
  public:

    delegate_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the delegate is uninitialised.
  //***************************************************************************
  class delegate_uninitialised : public delegate_exception
  {
  public:

    delegate_uninitialised(string_type file_name_, numeric_type line_number_)
      : delegate_exception(ETL_ERROR_TEXT("delegate:uninitialised", ETL_DELEGATE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //*****************************************************************
  /// The tag to identify an etl::delegate.
  ///\ingroup delegate
  //*****************************************************************
  struct delegate_tag
  {
  };

  //***************************************************************************
  /// is_delegate
  //***************************************************************************
  template <typename T>
  struct is_delegate : etl::bool_constant<etl::is_base_of<delegate_tag, T>::value>
  {
  };

#if ETL_USING_CPP17
  template <typename T>
  inline constexpr bool is_delegate_v = is_delegate<T>::value;
#endif

  //*************************************************************************
  /// Declaration.
  //*************************************************************************
  template <typename T>
  class delegate;

  //*************************************************************************
  /// Specialisation.
  //*************************************************************************
  template <typename TReturn, typename... TParams>
  class delegate<TReturn(TParams...)> final : public delegate_tag
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate() ETL_NOEXCEPT
    {
    }

    //*************************************************************************
    // Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate(const delegate& other) = default;

    //*************************************************************************
    // Construct from lambda or functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate(TLambda& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    // Construct from const lambda or functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate(const TLambda& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), const_lambda_stub<TLambda>);
    }

    //*************************************************************************
    // Delete construction from rvalue reference lambda or functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !etl::is_same<etl::delegate<TReturn(TParams...)>, TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate(TLambda&& instance) = delete;

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(ETL_NULLPTR, function_stub<Method>);
    }

    //*************************************************************************
    /// Create from Lambda or Functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(TLambda& instance) ETL_NOEXCEPT
    {
      return delegate((void*)(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Create from const Lambda or Functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(const TLambda& instance) ETL_NOEXCEPT
    {
      return delegate((void*)(&instance), const_lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(T& instance) ETL_NOEXCEPT
    {
      return delegate((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    /// Deleted for rvalue references.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(T&& instance) = delete;

    //*************************************************************************
    /// Create from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(const T& instance) ETL_NOEXCEPT
    {
      return delegate((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Disable create from rvalue instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const>
    static ETL_CONSTEXPR14 delegate create(T&& instance) = delete;

    //*************************************************************************
    /// Create from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::*Method)(TParams...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Create from instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParams...), T& Instance>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::*Method)(TParams...) const>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(const_method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParams...) const, T const& Instance>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(const_method_instance_stub<T, Method, Instance>);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Create from instance function operator (Compile time).
    /// At the time of writing, GCC appears to have trouble with this.
    //*************************************************************************
    template <typename T, T& Instance>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(operator_instance_stub<T, Instance>);
    }
#endif

    //*************************************************************************
    /// Set from function (Compile time).
    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(ETL_NULLPTR, function_stub<Method>);
    }

    //*************************************************************************
    /// Set from Lambda or Functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 void set(TLambda& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Set from const Lambda or Functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 void set(const TLambda& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), const_lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Set from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParams...)>
    ETL_CONSTEXPR14 void set(T& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParams...) const>
    ETL_CONSTEXPR14 void set(T& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::* Method)(TParams...)>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(ETL_NULLPTR, method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Set from instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParams...), T& Instance>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(ETL_NULLPTR, method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Set from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::* Method)(TParams...) const>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(ETL_NULLPTR, const_method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Set from const instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParams...) const, T const& Instance>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(ETL_NULLPTR, const_method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Clear the delegate.
    //*************************************************************************
    ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
    {
      invocation.clear();
    }

    //*************************************************************************
    /// Execute the delegate.
    //*************************************************************************
    ETL_CONSTEXPR14 TReturn operator()(TParams... args) const ETL_NOEXCEPT_EXPR(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(delegate_uninitialised));

      return (*invocation.stub)(invocation.object, etl::forward<TParams>(args)...);
    }

    //*************************************************************************
    /// Execute the delegate if valid.
    /// 'void' return delegate.
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14
    typename etl::enable_if_t<etl::is_same<TRet, void>::value, bool>
      call_if(TParams... args) const ETL_NOEXCEPT
    {
      if (is_valid())
      {
        (*invocation.stub)(invocation.object, etl::forward<TParams>(args)...);
        return true;
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    /// Execute the delegate if valid.
    /// Non 'void' return delegate.
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14
    typename etl::enable_if_t<!etl::is_same<TRet, void>::value, etl::optional<TReturn>>
      call_if(TParams... args) const ETL_NOEXCEPT
    {
      etl::optional<TReturn> result;

      if (is_valid())
      {
        result = (*invocation.stub)(invocation.object, etl::forward<TParams>(args)...);
      }

      return result;
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Run time alternative.
    //*************************************************************************
    template <typename TAlternative>
    ETL_CONSTEXPR14 TReturn call_or(TAlternative alternative, TParams... args) const ETL_NOEXCEPT
    {
      if (is_valid())
      {
        return (*invocation.stub)(invocation.object, etl::forward<TParams>(args)...);
      }
      else
      {
        return alternative(etl::forward<TParams>(args)...);
      }
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Compile time alternative.
    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    ETL_CONSTEXPR14 TReturn call_or(TParams... args) const ETL_NOEXCEPT
    {
      if (is_valid())
      {
        return (*invocation.stub)(invocation.object, etl::forward<TParams>(args)...);
      }
      else
      {
        return (Method)(etl::forward<TParams>(args)...);
      }
    }

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    delegate& operator =(const delegate& rhs) = default;

    //*************************************************************************
    /// Create from Lambda or Functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate& operator =(TLambda& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), lambda_stub<TLambda>);
      return *this;
    }

    //*************************************************************************
    /// Create from const Lambda or Functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate& operator =(const TLambda& instance) ETL_NOEXCEPT
    {
      assign((void*)(&instance), const_lambda_stub<TLambda>);
      return *this;
    }

    //*************************************************************************
    /// Checks equality.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 bool operator == (const delegate& rhs) const ETL_NOEXCEPT
    {
      return invocation == rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    ETL_CONSTEXPR14 bool operator != (const delegate& rhs) const ETL_NOEXCEPT
    {
      return invocation != rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
    {
      return invocation.stub != ETL_NULLPTR;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 operator bool() const ETL_NOEXCEPT
    {
      return is_valid();
    }

  private:

    using stub_type = TReturn(*)(void* object, TParams...);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      invocation_element() = default;

      //***********************************************************************
      ETL_CONSTEXPR14 invocation_element(void* object_, stub_type stub_) ETL_NOEXCEPT
        : object(object_)
        , stub(stub_)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR14 bool operator ==(const invocation_element& rhs) const ETL_NOEXCEPT
      {
        return (rhs.stub == stub) && (rhs.object == object);
      }

      //***********************************************************************
      ETL_CONSTEXPR14 bool operator !=(const invocation_element& rhs) const ETL_NOEXCEPT
      {
        return (rhs.stub != stub) || (rhs.object != object);
      }

      //***********************************************************************
      ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
      {
        object = ETL_NULLPTR;
        stub   = ETL_NULLPTR;
      }

      //***********************************************************************
      void*     object = ETL_NULLPTR;
      stub_type stub   = ETL_NULLPTR;
    };

    //*************************************************************************
    /// Constructs a delegate from an object and stub.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate(void* object, stub_type stub) ETL_NOEXCEPT
      : invocation(object, stub)
    {
    }

    //*************************************************************************
    /// Constructs a delegate from a stub.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate(stub_type stub) ETL_NOEXCEPT
      : invocation(ETL_NULLPTR, stub)
    {
    }

    //*************************************************************************
    /// Assign from an object and stub.
    //*************************************************************************
    ETL_CONSTEXPR14 void assign(void* object, stub_type stub) ETL_NOEXCEPT
    {
      invocation.object = object;
      invocation.stub   = stub;
    }

    //*************************************************************************
    /// Stub call for a member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...)>
    static ETL_CONSTEXPR14 TReturn method_stub(void* object, TParams... params) ETL_NOEXCEPT
    {
      T* p = static_cast<T*>(object);
      return (p->*Method)(etl::forward<TParams>(params)...);
    }

    //*************************************************************************
    /// Stub call for a const member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const>
    static ETL_CONSTEXPR14 TReturn const_method_stub(void* object, TParams... params) ETL_NOEXCEPT
    {
      T* const p = static_cast<T*>(object);
      return (p->*Method)(etl::forward<TParams>(params)...);
    }

    //*************************************************************************
    /// Stub call for a member function. Compile time instance.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...), T& Instance>
    static ETL_CONSTEXPR14 TReturn method_instance_stub(void*, TParams... params) ETL_NOEXCEPT
    {
      return (Instance.*Method)(etl::forward<TParams>(params)...);
    }

    //*************************************************************************
    /// Stub call for a const member function. Compile time instance.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const, const T& Instance>
    static ETL_CONSTEXPR14 TReturn const_method_instance_stub(void*, TParams... params) ETL_NOEXCEPT
    {
      return (Instance.*Method)(etl::forward<TParams>(params)...);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Stub call for a function operator. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance>
    static ETL_CONSTEXPR14 TReturn operator_instance_stub(void*, TParams... params) ETL_NOEXCEPT
    {
      return Instance.operator()(etl::forward<TParams>(params)...);
    }
#endif

    //*************************************************************************
    /// Stub call for a free function.
    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    static ETL_CONSTEXPR14 TReturn function_stub(void*, TParams... params) ETL_NOEXCEPT
    {
      return (Method)(etl::forward<TParams>(params)...);
    }

    //*************************************************************************
    /// Stub call for a lambda or functor function.
    //*************************************************************************
    template <typename TLambda>
    static ETL_CONSTEXPR14 TReturn lambda_stub(void* object, TParams... arg) ETL_NOEXCEPT
    {
      TLambda* p = static_cast<TLambda*>(object);
      return (p->operator())(etl::forward<TParams>(arg)...);
    }

    //*************************************************************************
    /// Stub call for a const lambda or functor function.
    //*************************************************************************
    template <typename TLambda>
    static ETL_CONSTEXPR14 TReturn const_lambda_stub(void* object, TParams... arg) ETL_NOEXCEPT
    {
      const TLambda* p = static_cast<const TLambda*>(object);
      return (p->operator())(etl::forward<TParams>(arg)...);
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };

#if ETL_USING_CPP17
  //*************************************************************************
  /// Make a delegate from a free function.
  //*************************************************************************
  template <auto Function>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Function)>::function_type;

    return etl::delegate<function_type>::template create<Function>();
  }

  //*************************************************************************
  /// Make a delegate from a functor or lambda function.
  //*************************************************************************
  template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value, void>>
  ETL_NODISCARD
  constexpr auto make_delegate(TLambda& instance) ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(&TLambda::operator())>::function_type;

    return etl::delegate<function_type>(instance);
  }

  //*************************************************************************
  /// Make a delegate from a functor, compile time.
  //*************************************************************************
  template <typename T, T& Instance>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(&T::operator())>::function_type;

    return etl::delegate<function_type>::template create<T, Instance>();
  }

  //*************************************************************************
  /// Make a delegate from a member function at compile time.
  //*************************************************************************
  template <typename T, auto Method, T& Instance, typename = etl::enable_if_t<!etl::function_traits<decltype(Method)>::is_const>>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method, Instance>();
  }

  //*************************************************************************
  /// Make a delegate from a const member function at compile time.
  //*************************************************************************
  template <typename T, auto Method, const T& Instance, typename = etl::enable_if_t<etl::function_traits<decltype(Method)>::is_const>>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method, Instance>();
  }

  //*************************************************************************
  /// Make a delegate from a member function at run time.
  //*************************************************************************
  template <typename T, auto Method>
  ETL_NODISCARD
  constexpr auto make_delegate(T& instance) ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method>(instance);
  }

  //*************************************************************************
  /// Make a delegate from a member function at run time.
  //*************************************************************************
  template <typename T, auto Method>
  ETL_NODISCARD
  constexpr auto make_delegate(const T& instance) ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method>(instance);
  }
#endif
}

#endif
