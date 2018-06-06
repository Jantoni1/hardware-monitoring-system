#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>
#include <boost/lexical_cast.hpp>
class configuration
{
public:
	configuration() 
		: id_(-1)
		, identification_name_("undefined")
		, id_from_server_(-1)
		, authorization_passed_(false)
		, tcp_server_ip_address_("127.0.0.1")
		, tcp_server_port_("7002")
		, tcp_daemon_ip_address_("127.0.0.1")
		, tcp_daemon_port_("7003")
		, udp_server_ip_address_("127.0.0.1")
		, udp_server_port_("7002")
		, configuration_file_path_("")

	{}

	~configuration();

	void parse_configuration();

	int32_t id() const
	{
		return id_;
	}

	void set_id_from_string(std::string& id)
	{
		id_ = boost::lexical_cast<int>(id);
	}

	void set_id(const int32_t id)
	{
		id_ = id;
	}

	std::string tcp_server_ip_address() const
	{
		return tcp_server_ip_address_;
	}

	void set_tcp_server_ip_address(const std::string& tcp_server_ip_address);

	std::string tcp_server_port() const
	{
		return tcp_server_port_;
	}

	void set_tcp_server_port(const std::string& tcp_server_port);

	std::string tcp_daemon_ip_address() const
	{
		return tcp_daemon_ip_address_;
	}

	void set_tcp_daemon_ip_address(const std::string& tcp_daemon_ip_address);

	std::string tcp_daemon_port() const
	{
		return tcp_daemon_port_;
	}

	void set_tcp_daemon_port(const std::string& tcp_daemon_port);

	std::string udp_server_ip_address() const
	{
		return udp_server_ip_address_;
	}

	void set_udp_server_ip_address(const std::string& udp_server_ip_address);

	std::string udp_server_port() const
	{
		return udp_server_port_;
	}

	void set_udp_server_port(const std::string& udp_server_port);

	std::string identification_name() const
	{
		return identification_name_;
	}

	void set_identification_name(const std::string& identification_name);


	int32_t id_from_server() const
	{
		return id_from_server_;
	}

	void set_id_from_server(const int32_t id_from_server)
	{
		id_from_server_ = id_from_server;
	}


	bool authorization_passed() const
	{
		return authorization_passed_;
	}

	void set_authorization_passed(const bool authorization_passed)
	{
		authorization_passed_ = authorization_passed;
	}


	std::string configuration_file_path() const
	{
		return configuration_file_path_;
	}

	void set_configuration_file_path(const std::string& configuration_file_path)
	{
		configuration_file_path_ = configuration_file_path;
	}


	std::string secret() const
	{
		return secret_;
	}


	void set_secret(const std::string& secret)
	{
		secret_ = secret;
	}

private:
	void check_default_values_changed() const;
	void parse_line(const std::string& line);
	void parse_to_ip_address(const std::string &ip) const;

	// id from configuration, used to identification only once with hello message
	int32_t id_; 
	
	//name from configuration file, sent with hello message to server,
	//allows server to find this device in database
	std::string identification_name_;

	// id received form server, used in this particular session to identify messages from this device on server
	int32_t id_from_server_;

	bool authorization_passed_;

	std::string tcp_server_ip_address_;
	std::string tcp_server_port_;

	std::string tcp_daemon_ip_address_;
	std::string tcp_daemon_port_;

	std::string udp_server_ip_address_;
	std::string udp_server_port_;

	std::string configuration_file_path_;

	std::string secret_;

	const std::unordered_map<std::string, std::function<void(std::string&)>> parser_map_ = 
		{ { "identification_name",  std::bind(&configuration::set_identification_name, this, std::placeholders::_1)			}
		, { "server_auth_ip"		 ,	std::bind(&configuration::set_tcp_server_ip_address, this, std::placeholders::_1)	}
		, { "server_auth_port"	 ,	std::bind(&configuration::set_tcp_server_port, this, std::placeholders::_1)				}
		, { "daemon_ip"			 ,	std::bind(&configuration::set_tcp_daemon_ip_address, this, std::placeholders::_1)		}
		, { "daemon_port"		 ,	std::bind(&configuration::set_tcp_daemon_port, this, std::placeholders::_1)				}
		, { "server_data_ip"		 ,	std::bind(&configuration::set_udp_server_ip_address, this, std::placeholders::_1)	}
		, { "server_data_port"	 ,	std::bind(&configuration::set_udp_server_port, this, std::placeholders::_1)				}
		, { "id"				 ,	std::bind(&configuration::set_id_from_string, this, std::placeholders::_1)				}
		, { "secret"			 ,  std::bind(&configuration::set_secret, this, std::placeholders::_1)						}		
	};
		 
		 
		 
			
		 
			
};

