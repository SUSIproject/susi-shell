#include "leveldb/db.h"
#include "susi/SusiClient.h"
#include "susi/sha3.h"
#include <uuid/uuid.h>
#include <regex>

class Authenticator {
public:
	Authenticator(std::string addr,short port, std::string key, std::string cert);
	void join();

protected:

	struct User {
		std::string name;
		std::string pwHash;
		std::string token;
		std::vector<std::string> roles;
	};

	struct Permission {
		Susi::Event pattern;
		std::vector<std::string> roles;
	};

	std::map<std::string,std::shared_ptr<User>> usersByName;
	std::map<std::string,std::shared_ptr<User>> usersByToken;
	std::map<std::string,Permission> permissionsByTopic;

	void addUser(std::shared_ptr<User> user);
	void addPermission(Permission permission);
	void load();
	void save();

	std::string generateToken();
	std::string getTokenFromEvent(const Susi::EventPtr & event);

	void login(Susi::EventPtr event);
	void logout(Susi::EventPtr event);
	void registerGuard(Permission permission);

	bool checkIfPayloadMatchesPattern(BSON::Value pattern, BSON::Value payload);

	std::shared_ptr<Susi::SusiClient> susi_;
};