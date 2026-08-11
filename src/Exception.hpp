#pragma once
#define PCL_EXCEPTION

#include<exception>
#include<vector>
#include<print>
#include<functional>

namespace pc
{

#define pcXPT_DEFAULT_CONTENT "An uncaught exception from PClib thrown"
#define pcXPT_DEFAULT_TYPE "exception"

#define pcXPT_ASSERTION_FAILED "AssertionFailed"
#define pcXPT_TIMEOUT "Timeout"
#define pcXPT_BUSY "BusyError"
#define pcXPT_BAD_ALLOC "BadAlloc"
#define pcXPT_BAD_CAST "BadCast"
#define pcXPT_BAD_TYPEID "BadTypeid"
#define pcXPT_BAD_EXCEPTION "BadException"
#define pcXPT_DIVIDE_BY_ZERO "DivideByZero"
#define pcXPT_DOMAIN "DomainError"
#define pcXPT_FILE "FileError"
#define pcXPT_FILE_NOT_FOUND "FileNotFound"
#define pcXPT_INIT "NotInitiated"
#define pcXPT_INVALID_ARGUMENT "InvalidArgument"
#define pcXPT_IO "IoError"
#define pcXPT_LENGTH "LengthError"
#define pcXPT_LOGIC "LogicError"
#define pcXPT_NOT_FOUND "NotFoundError"
#define pcXPT_NULL_POINTER "NullPointer"
#define pcXPT_OUT_OF_RANGE "OutOfRange"
#define pcXPT_OVERFLOW "OverflowError"
#define pcXPT_PARSING "ParsingError"
#define pcXPT_SYSTEM "SystemError"
#define pcXPT_RANGE "RangeError"
#define pcXPT_RUNTIME "RuntimeError"
#define pcXPT_UNDERFLOW "UnderflowError"
#define pcXPT_UNKNOWN "UnknownError"

class ExceptionType
{
	private:
		std::vector<std::string> Type;
		static bool validType(std::string tp)
		{
			if(tp.empty())	return false;
			for(auto c:tp)
				if(!isalnum(c)&&c!='_')	return false;
			return true;
		}
		static std::string toLower(std::string s)
		{
			for(auto& c:s)
				c=tolower(c);
			return s;
		}
	public:
		ExceptionType(std::string tp)
		{
			std::string t;
			int i=0,s=tp.length();
			for(int i=0;i<s;i++)
				if(tp[i]=='.'||i==s-1)
				{
					if(i==s-1)	t+=tp[i];
					if(!validType(t))
						throw std::runtime_error(std::vformat("[" pcXPT_INVALID_ARGUMENT "]Invalid exception type: {}",std::make_format_args(t)));
					Type.push_back(toLower(t));
					t.clear();
				}
				else	t+=tp[i];
		}
		ExceptionType(const char* tp):ExceptionType(std::string(tp)){}
		ExceptionType(){Type.push_back(pcXPT_DEFAULT_TYPE);}
		ExceptionType(const ExceptionType& other){Type=other.Type;}
		ExceptionType& operator=(const ExceptionType& other){Type=other.Type;return *this;}
		std::string ToString() const
		{
			std::string ans="";
			int i=0;
			for(auto s:Type)
				ans+=(i==0)?s:"."+s,i++;
			return ans;
		}
		/// @brief Get a subtype of this type, with the subtype name `sub` 
		ExceptionType SubType(std::string sub) const
		{
			ExceptionType ans=*this;
			ans.Type.push_back(toLower(sub));
			return ans;
		}
		/// @brief Get the parent type of this type, if it is the root type, return itself. 
		ExceptionType ParentType() const
		{
			ExceptionType ans=*this;
			if(!ans.Type.empty())
				ans.Type.pop_back();
			return ans;
		}
		bool operator==(const ExceptionType& other) const
		{
			if(Type.size()!=other.Type.size())	return false;
			for(int i=0;i<Type.size();i++)
				if(Type[i]!=other.Type[i])	return false;
			return true;
		}
		bool operator!=(const ExceptionType& other) const {return !(*this==other);}
		/// @brief Check if this type is a supertype of `other` 
		bool operator>>(const ExceptionType& other) const
		{
			if(Type.size()>other.Type.size())	return false;
			for(int i=0;i<Type.size();i++)
				if(Type[i]!=other.Type[i])	return false;
			return true;
		}
		/// @brief Check if this type is a subtype of `other`
		bool operator<<(const ExceptionType& other) const
		{
			if(Type.size()<other.Type.size())	return false;
			for(int i=0;i<other.Type.size();i++)
				if(Type[i]!=other.Type[i])	return false;
			return true;
		}
};

/**
 * @brief This is the exception class thrown by PCLib.
 * @warning If an exception is not handled before destruction, the program will terminate.
 * So remember to capture it by **reference**(`pc::Exception&`)!
 * @note use like this:
 * ```cpp
 * try{
 * 	// code that may throw exception
 * }catch(pc::Exception& e)
 * {
 * 	e.HandleIf("test",[](pc::Exception& e){
 * 		// handle exception of type "test"
 * 	}).HandleIf("other",[](pc::Exception& e){
 * 		// handle exception of type "other"
 * 	}).HandleAnyway(); // set it handled whaterver
 * }
 */
class Exception:public std::runtime_error
{
private:
	bool handled=false;
	explicit Exception(const Exception& other)
		:runtime_error(other.message)
	{
		type=other.type;
		message=other.message;
	};
protected:
	std::string message;
	ExceptionType type;
public:
	/**
	 * @brief Default constructor with type=`pcXPT_DEFAULT_TYPE`, 
	 * content=pcXPT_DEFAULT_CONTENT
	 */
	explicit Exception():std::runtime_error(pcXPT_DEFAULT_CONTENT),
		message(pcXPT_DEFAULT_CONTENT),type(pcXPT_DEFAULT_TYPE){}
	/**
	 * @brief Constructor for exceptions that PClib throws.
	 * @param tp Type of the exception
	 * @param fmt Format string for the content
	 * @param args Arguments for the format string
	 * @note The type system supports inheritance, using `.` to connect type names.
	 * ```cpp
	 * pc::Exception("system","Message in what()");
	 * pc::Exception("system.io.overload","This a {} exception","test");
	 * ```
	 */
	template<typename ...Tps>
	explicit Exception(ExceptionType tp,std::string fmt,Tps ...args)
		:std::runtime_error(pcXPT_DEFAULT_CONTENT)
	{
		message=std::vformat(fmt,std::make_format_args(args...));
		message=std::format("[{}] {}",type.ToString(),message);
		type=tp;
	}
	explicit Exception(Exception&&) = default;
	Exception& operator=(const Exception&) = delete;
	const char* what() const noexcept override
		{return message.c_str();}
	const std::string Message() const noexcept {return message;}
	ExceptionType Type() const noexcept {return type;}
	/// @brief Handle the exception if it is of type `tp`, otherwise do nothing. 
	Exception& HandleIf(ExceptionType tp,std::function<void(Exception&)> handler)
	{
		if(!handled&&tp>>this->type)
			handler(*this),
			handled=true;
		return *this;
	}
	/// @brief Functions literally.
	void ThrowIfUnhandled(){
		if(!handled)	throw Exception(*this);
	}
	/// @brief Mark the exception as handled, so it will not terminate the program when destroyed.
	void HandleAnyway(){handled=true;}
	/// @brief Destructor, terminate the program if the exception is not handled.
	~Exception()
	{
		if(handled)	return;
		std::print(stderr,"Uncaught exception instance pc::Exception\nwhat(): {}",what());
		std::cerr.flush();
		std::terminate();
	}
};


};//namespace