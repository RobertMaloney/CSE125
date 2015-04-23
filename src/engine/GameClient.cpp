#include "GameClient.h"


GameClient::GameClient() {
    connection = new TCPConnection();
}


GameClient::~GameClient() {
    if (connection) {
        delete connection;
        connection = nullptr;
    }
}


// used for debugging
void fillWithDebugPackets(deque<Packet> & packets)
{
	Packet p;
	for (int i = 0; i < 26; ++i) {
		p.Resize(0);
		int len = rand() % 1000;
		for (int j = 0; j < len; ++j) {
			p.WriteChar('a' + i);
		}
		packets.push_back(p);
	}
}


void GameClient::run()
{
	bool DEBUG = true;

	GraphicsEngine::Initialize();
	//GraphicsEngine::SetKeyCallback(keyCallback);
	//client = new GameClient();
	//client->Initialize();
	this->Initialize();

	deque<Packet> packets;
	deque<Packet> updates;
	fillWithDebugPackets(packets);
	Packet p;

	int i = 0;

	//main game loop
	while (!GraphicsEngine::Closing()) {
		GraphicsEngine::DrawAndPoll();
		if (DEBUG) {

			//send user input
			this->SendEvents(GameClient::input);

			this->ReceiveUpdates(updates);
			//client->PrintUpdates(updates);
			GraphicsEngine::UpdatePlayer(updates);

			//clear events
			updates.clear();
			GameClient::input.clear();
		}
	}

	GraphicsEngine::Destroy();
	system("pause");
}


void GameClient::Initialize() {
    Socket::Initialize();
    SocketError err = connection->Connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
    if (ShouldTerminate(err)) {
        connection->Close();
        delete connection;
        connection = nullptr;
        throw SocketException("Connection Failure.");
    }
    connection->SetNoDelay(true);
    connection->SetNonBlocking(true);
}


void GameClient::ReceiveUpdates(deque<Packet> & updates) {
    this->CheckError(connection->Receive(updates));
}


void GameClient::SendEvents(deque<Packet> & events) {
    this->CheckError(connection->Send(events));    
}


void GameClient::PrintUpdates(deque<Packet> & updates) {
    for (auto it = updates.begin(); it != updates.end(); ++it) {
        for (unsigned int i = 0; i < it->Size(); ++it) {
            cout << to_string(it->At(i)) << " ";
        }
        cout << "\n";
    }
}


void GameClient::CheckError(SocketError err) {
	if (this->ShouldTerminate(err)) {
		connection->Close();
		throw SocketException("Fatal error while communicating over TCPConnection.");
	}
}


bool GameClient::ShouldTerminate(SocketError err) {
	switch (err) {
	case SE_NOERR:
		return false;
	case SE_WOULDBLOCK:
		return false;
	case SE_NODATA:
		return false;
	default:
		return true;
	}
}
