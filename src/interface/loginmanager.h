#ifndef FILEZILLA_INTERFACE_LOGINMANAGER_HEADER
#define FILEZILLA_INTERFACE_LOGINMANAGER_HEADER

#include "serverdata.h"

#include <list>

// The purpose of this class is to manage some aspects of the login
// behaviour. These are:
// - Password dialog for servers with ASK or INTERACTIVE logontype
// - Storage of passwords for ASK servers for duration of current session

class CLoginManager
{
public:
	static CLoginManager& Get() { return m_theLoginManager; }

	bool GetPassword(ServerWithCredentials& server, bool silent, std::wstring const& name = std::wstring());
	bool GetPassword(ServerWithCredentials& server, bool silent, std::wstring const& name, std::wstring const& challenge, bool canRemember);

	void CachedPasswordFailed(CServer const& server, std::wstring const& challenge = std::wstring());

	void RememberPassword(ServerWithCredentials & server, std::wstring const& challenge = std::wstring());

	bool AskDecryptor(fz::public_key const& pub, bool allowForgotten, bool allowCancel);
	fz::private_key GetDecryptor(fz::public_key const& pub);

protected:
	bool DisplayDialogForEncrypted(ServerWithCredentials& server, std::wstring const& name);
	bool DisplayDialog(ServerWithCredentials& server, std::wstring const& name, std::wstring const& challenge, bool canRemember);

	static CLoginManager m_theLoginManager;

	// Session password cache for Ask-type servers
	struct t_passwordcache
	{
		std::wstring host;
		unsigned int port;
		std::wstring user;
		std::wstring password;
		std::wstring challenge;
	};

	std::list<t_passwordcache>::iterator FindItem(CServer const& server, std::wstring const& challenge);

	std::list<t_passwordcache> m_passwordCache;

	std::map<fz::public_key, fz::private_key> decryptors_;
};

#endif
