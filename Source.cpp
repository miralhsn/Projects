#include <iostream>
#include <string>
using namespace std;

class User {
protected:
	string username;
	string password;
public:
	User()
	{
		username = "";
		password = "";
	}
	User(string u, string p)
	{
		username = u;
		password = p;
	}
	User(User &u)
	{
		u.password = password;
		u.username = username;
	}
	void setUsername(string u)
	{
		this->username = u;
	}
	string getUsername()
	{
		return username;
	}
	void setPassword(string p)
	{
		this->password = p;
	}
	string getPassword()
	{
		return password;
	}
};
