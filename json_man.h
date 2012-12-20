#ifndef JSON_MAN_H_
#define JSON_MAN_H_

#include <string.h>
#include "json/json.h"


class JsonMan
{
public:
	JsonMan():ready_(false){}
	~JsonMan(){}

	bool isAvail()
	{
		return ready_;
	}

	bool set(const std::string& data);
	void operator>>(std::string& jsonstr);
	void operator<<(const std::string& jsonstr);

   /** \brief Experimental and untested: represents a "path" to access a node.
    *
    * Syntax:
    * - "." => root node
    * - ".[n]" => elements at index 'n' of root node (an array value)
    * - ".name" => member named 'name' of root node (an object value)
    * - ".name1.name2.name3"
    * - ".[0][1][2].name1[3]"
    * - ".%" => member name is provided as parameter
    * - ".[%]" => index is provied as parameter
    */

/*
 * return:
 * 	0 success
 * 	-1 path not found
 * 	-2 check consistency failed
 * 	-3 unavailable new_data
 */
	int update(const std::string& path,const std::string& new_data,const std::string& old_data);
/*
 * return:
 * 	0 success
 * 	-1 path not found
 */
	int getSub(const std::string& path,std::string& cfg);
	void get(const std::string& path,Json::Value& v);
	
private:
	Json::Value& get(const std::string& path);

private:
	//std::string m_data;
	Json::Value root_;
	bool ready_;
};



#endif
