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
#define pcXPT_DEFAULT_OWNER "univeral"

class ExceptionType
{
private:
	std::vector<std::string> Type;
	static bool validType(std::string tp)
	{
		if(tp.empty())	return false;
		for(auto c:tp)
			if(!isalnum(c)&&c!='_'&&c!='.')	return false;
		return true;
	}
	static std::string toLower(std::string s)
	{
		for(auto& c:s)
			c=tolower(c);
		return s;
	}
public:
	ExceptionType(std::string_view tp)
	{
		std::string t;
		int i=0,s=tp.length();
		for(int i=0;i<s;i++)
			if(tp[i]=='.'||i==s-1)
			{
				if(i==s-1)	t+=tp[i];
				if(!validType(t))
					throw std::runtime_error(std::vformat("Invalid exception type: '{}'",
						std::make_format_args(t)));
				Type.push_back(t);
				t.clear();
			}
			else	t+=tp[i];
	}
	ExceptionType(const char* tp):ExceptionType(std::string_view(tp)){}
	ExceptionType(std::string tp):ExceptionType(std::string_view(tp)){}
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
		ans.Type.push_back(sub);
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
		if(Type.size()!=other.Type.size())
			return false;
		for(int i=0;i<Type.size();i++)
			if(toLower(Type[i])!=toLower(other.Type[i]))
				return false;
		return true;
	}
	bool operator!=(const ExceptionType& other) const {return !(*this==other);}
	/// @brief Check if this type is a supertype of `other` 
	bool operator>>(const ExceptionType& other) const
	{
		if(Type.size()>other.Type.size())
			return false;
		for(int i=0;i<Type.size();i++)
			if(toLower(Type[i])!=toLower(other.Type[i]))
				return false;
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

#define pcXPT_ASSERTION pc::ExceptionType("AssertionFailed")
#define pcXPT_ACCESS_DENIED pc::ExceptionType("AccessDenied")
#define pcXPT_TIMEOUT pc::ExceptionType("Timeout")
#define pcXPT_BUSY pc::ExceptionType("BusyError")
#define pcXPT_BAD_ALLOC pc::ExceptionType("BadAlloc")
#define pcXPT_BAD_CAST pc::ExceptionType("BadCast")
#define pcXPT_BAD_TYPEID pc::ExceptionType("BadTypeid")
#define pcXPT_BAD_EXCEPTION pc::ExceptionType("BadException")
#define pcXPT_DIVIDE_BY_ZERO pc::ExceptionType("DivideByZero")
#define pcXPT_DOMAIN pc::ExceptionType("DomainError")
#define pcXPT_FILE pc::ExceptionType("FileError")
#define pcXPT_FILE_NOT_FOUND pc::ExceptionType("FileNotFound")
#define pcXPT_INIT pc::ExceptionType("NotInitiated")
#define pcXPT_INVALID_ARGUMENT pc::ExceptionType("InvalidArgument")
#define pcXPT_IO pc::ExceptionType("IoError")
#define pcXPT_LENGTH pc::ExceptionType("LengthError")
#define pcXPT_LOGIC pc::ExceptionType("LogicError")
#define pcXPT_NOT_FOUND pc::ExceptionType("NotFoundError")
#define pcXPT_NULL_POINTER pc::ExceptionType("NullPointer")
#define pcXPT_OUT_OF_RANGE pc::ExceptionType("OutOfRange")
#define pcXPT_OVERFLOW pc::ExceptionType("OverflowError")
#define pcXPT_PARSING pc::ExceptionType("ParsingError")
#define pcXPT_PERMISSION pc::ExceptionType("PermissionError")
#define pcXPT_SYSTEM pc::ExceptionType("SystemError")
#define pcXPT_RANGE pc::ExceptionType("RangeError")
#define pcXPT_RUNTIME pc::ExceptionType("RuntimeError")
#define pcXPT_UNDERFLOW pc::ExceptionType("UnderflowError")
#define pcXPT_UNKNOWN pc::ExceptionType("UnknownError")

class ExceptionTypes{
private:
	std::vector<ExceptionType> types;
	void add(ExceptionType tp){types.push_back(tp);}
	void makeWithString(std::string_view str){
		std::string one="";
		for(char i:str)
			if(i==' ')
				if(!one.empty())
					types.emplace_back(one),
					one="";
				else;
			else	one+=i;
		if(!one.empty())
			types.emplace_back(one);
	}
public:
	ExceptionTypes(){types.emplace_back(pcXPT_DEFAULT_TYPE);}
	ExceptionTypes(ExceptionType tp){types.push_back(tp);}
	ExceptionTypes(std::string_view str){makeWithString(str);}
	ExceptionTypes(const char* str){makeWithString(str);}
	ExceptionTypes(std::string str){makeWithString(str);}
	inline friend ExceptionTypes operator+(ExceptionTypes a,ExceptionTypes b){
		for(ExceptionType e:b.types)
			a.add(e);
		return a;
	}
	inline friend ExceptionTypes operator+(ExceptionTypes a,ExceptionType b)
		{a.add(b);return a;}
	inline ExceptionTypes operator+=(ExceptionTypes other){
		for(ExceptionType e:other.types)
			add(e);
		return *this;
	}
	inline ExceptionTypes operator+=(ExceptionType other)
		{add(other);return *this;}
	bool canCaughtBy(ExceptionType tp){
		for(ExceptionType& e:types)
			if(e<<tp)	return true;
		return false;
	}
	bool canCaughtBy(ExceptionTypes tps){
		for(ExceptionType& i:types)
			for(ExceptionType& j:tps.types)
				if(i<<j)	return true;
		return false;
	}
	std::string ToString() const
	{
		std::string ans="";
		int i=0;
		for(auto s:types)
			ans+=((i==0)?"":" ")+s.ToString(),i++;
		return ans;
	}
};
inline ExceptionTypes operator+(ExceptionType a,ExceptionType b){
	ExceptionTypes ret(a);
	ret+=b;
	return ret;
}

/**
 * @brief This is the exception class thrown by PCLib.
 * Of course you can use it if you what.
 * @warning If an exception is not handled before destruction, the program will terminate.
 * So remember to capture it by **reference**(`pc::Exception&`)!
 * @note This class inherits `std::runtime_error`.
 * Use like this:
 * ```cpp
 * try{
 * 	// code that may throw exception
 * }catch(pc::Exception& e)
 * {
 * 	e.HandleIf("test debug.try",[](pc::Exception& e){
 * 		// handle exception of type "test" and "debug.try"
 * 	}).HandleIf("other",[](pc::Exception& e){
 * 		// handle exception of type "other"
 * 	}).HandleAnyway(); // set it handled whaterver
 * }
 */
class Exception: public std::runtime_error
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
	ExceptionTypes type;
public:
	/**
	 * @brief Default constructor with type=`pcXPT_DEFAULT_TYPE`, 
	 * content=`pcXPT_DEFAULT_CONTENT`
	 */
	explicit Exception():
		std::runtime_error(pcXPT_DEFAULT_CONTENT),
		message(pcXPT_DEFAULT_CONTENT),
		type(ExceptionType(pcXPT_DEFAULT_TYPE)){}
	/**
	 * @brief Constructor for exceptions that PClib throws.
	 * @param tp Type of the exception
	 * @param fmt Format string for the content
	 * @param args Arguments for the format string
	 * @note The type system supports inheritance, using `.` to connect type layers,
	 * and using space in literal or using `+` between variables to connect different types.
	 * ```cpp
	 * pc::Exception("system","Message in what()");
	 * pc::Exception("system.io.overload","This a {} exception","test");
	 * pc::Exception("system pclib"+pcXPT_UNKNOWN,"Multi types exception");
	 * ```
	 */
	template<typename ...Tps>
	explicit Exception(ExceptionTypes tps,std::string fmt,Tps ...args)
		:std::runtime_error(pcXPT_DEFAULT_CONTENT)
	{
		message=std::vformat(fmt,std::make_format_args(args...));
		message=std::format("[{}] {}",tps.ToString(),message);
		type=tps;
	}
	explicit Exception(Exception&&) = default;
	Exception& operator=(const Exception&) = delete;
	const char* what() const noexcept override
		{return message.c_str();}
	const std::string Message() const noexcept {return message;}
	ExceptionTypes Types() const noexcept {return type;}
	/// @brief Handle the exception if it is of type `tp`, otherwise do nothing. 
	Exception& HandleIf(ExceptionTypes tp,std::function<void(Exception&)> handler)
	{
		if(!handled&&type.canCaughtBy(tp))
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
		fflush(stderr);
		std::terminate();
	}
};

/// @brief To throw a specific type of `pc::Exception`
class Exceptioner
{
	ExceptionType type;
public:
	Exceptioner(ExceptionType tp):type(tp){}
	/// @brief Directly throw the exception
	template<typename ...Tps>
	inline void Throw(ExceptionTypes tps,std::string fmt,Tps ...args) const
		{throw Exception(type+tps,fmt,...args);}
};


};//namespace