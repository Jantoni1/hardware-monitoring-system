#pragma once
#include <functional>
#include "ReceivedMessage.h"
#include <unordered_map>
#include "AccessDenied.h"
#include "Connected.h"
#include "HelloChallenge.h"

enum class ReceivedMessageType {hello_challenge = 0, connected = 1, access_denied = 2};

class ReceiveMessageFactory
{
public:
	ReceiveMessageFactory() = default;
	~ReceiveMessageFactory() = default;

	ReceivedMessage* buildMessage(char* buffer, unsigned length) const;

private:

	AccessDenied* buildAccessDeniedMessage(char* buffer, unsigned length) const;
	Connected* buildConnectedMessage(char* buffer, unsigned length) const;
	HelloChallenge* buildHelloChallengeMessage(char* buffer, unsigned length) const;

	const std::unordered_map<ReceivedMessageType, std::function<ReceivedMessage*(char*, unsigned)>> factory_object_types_ =
	{ {ReceivedMessageType::hello_challenge, this->buildHelloChallengeMessage}
	, {ReceivedMessageType::connected, this->buildConnectedMessage}
	, {ReceivedMessageType::access_denied, this->buildAccessDeniedMessage} };
};

