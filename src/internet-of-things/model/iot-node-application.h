//
// Created by anyonepaw on 2019-03-26.
//

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/ipv4-address.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/bitcoin.h"
#include "../3rd-party/rapidjson/include/rapidjson/document.h"
#include "../3rd-party/rapidjson/include/rapidjson/writer.h"
#include "../3rd-party/rapidjson/include/rapidjson/stringbuffer.h"
#include <random>

namespace ns3 {

    class Address;

    class Socket;

    class Packet;

    class IotNode : public Application {
    public:
        /**
         * \brief Get the type ID.
         * \return the object TypeId
         */
        static TypeId GetTypeId(void);

        IotNode();


        virtual ~IotNode();

        /**
         * \return pointer to listening socket
         */
        Ptr<Socket> GetListeningSocket(void) const;

        /**
         * \return a vector containing the addresses of peers
         */
        std::vector<Ipv4Address> GetPeersAddresses(void) const;

        /**
         * \return the node's public key
         */
//        CryptoPP::RSA::PublicKey GetPublickey(void) const;
//
//        /**
//         * \return the node's public key
//         */
//        CryptoPP::RSA::PrivateKey GetPrivatekey(void) const;
//
//        /**
//         * \return the node's gateway public key
//         */
//        CryptoPP::RSA::PublicKey GetGatewayPublickey(void) const;
//
//        /**
//         * \brief Revoke and change the node's keys
//         *	\param new private key and public key to be updated.
//         */
//        void SetNewKeys(CryptoPP::RSA::PrivateKey newPrivKey, CryptoPP::RSA::PublicKey newPublicKey);
//
//        /**
//         * \brief Update the node's gateway public key
//         *	\param new public key of the gateway
//         */
//        void SetGatewayPublickey(CryptoPP::RSA::PublicKey newGatewayKey);

        /**
         * \brief Set the addresses of peers
         * \param peers the reference of a vector containing the Ipv6 addresses of peers
         */
        void SetPeersAddresses(const std::vector<Ipv4Address> &peers);

        /**
         * \brief adds a peer to the set of peers
         * \param newPeer address of the peer to be added
         **/
        void AddPeer(Ipv4Address newPeer);

        /**
         * \brief set the download speeds of peers
         * \param peersDownloadSpeeds the reference of a map containing the Ipv6 addresses of peers and their corresponding download speed
         */
        void SetPeersDownloadSpeeds(const std::map<Ipv4Address, double> &peersDownloadSpeeds);

        /**
         * \brief Set the upload speeds of peers
         * \param peersUploadSpeeds the reference of a map containing the Ipv6 addresses of peers and their corresponding upload speed
         */
        void SetPeersUploadSpeeds(const std::map<Ipv4Address, double> &peersUploadSpeeds);

        /**
         * \brief Set the internet speeds of the node
         * \param internetSpeeds a struct containing the download and upload speed of the node
         */
        void SetNodeInternetSpeeds(const nodeInternetSpeeds &internetSpeeds);

        /**
         * \brief Set the node statistics
         * \param nodeStats a reference to a nodeStatistics struct
         */
        void SetNodeStats(nodeStatistics *nodeStats);

        /**
         * \brief Set the protocol type(default: STANDARD_PROTOCOL)
         * \param protocolType the type of protocol used for advertising new blocks
         */
        void SetProtocolType(enum ProtocolType protocolType);

        /**
         * \brief Sends a message to a peer
         * \param receivedMessage the type of the received message
         * \param responseMessage the type of the response message
         * \param d the rapidjson document containing the info of the outgoing message
         * \param outgoingSocket the socket of the peer
         */
        void SendMessage(enum Messages receivedMessage, enum Messages responseMessage, rapidjson::Document &d,
                         Ptr<Socket> outgoingSocket);

        /**
         * \brief Sends a message to a peer
         * \param receivedMessage the type of the received message
         * \param responseMessage the type of the response message
         * \param d the rapidjson document containing the info of the outgoing message
         * \param outgoingAddress the Address of the peer
         */
        void SendMessage(enum Messages receivedMessage, enum Messages responseMessage, rapidjson::Document &d,
                         Address &outgoingAddress);

        /**
         * \brief Sends a message to a peer
         * \param receivedMessage the type of the received message
         * \param responseMessage the type of the response message
         * \param packet a string containing the info of the outgoing message
         * \param outgoingAddress the Address of the peer
         */
        void SendMessage(enum Messages receivedMessage, enum Messages responseMessage, std::string packet,
                         Address &outgoingAddress);

        /**
         * \brief Sends a message to a peer
         * \param receivedMessage the type of the received message
         * \param responseMessage the type of the response message
         * \param packet a string containing the info of the outgoing message
         * \param outgoingIpv4Address the Ipv4Address of the peer
         */
        void SendMessage(enum Messages receivedMessage, enum Messages responseMessage, std::string packet,
                         Ipv4Address &outgoingIpv4Address);

        /**
         * \brief Sends a message to a peer
         * \param receivedMessage the type of the received message
         * \param responseMessage the type of the response message
         * \param packet a string containing the info of the outgoing message
         * \param outgoingSocket the socket of the peer
         */
        void SendMessage(enum Messages receivedMessage, enum Messages responseMessage, std::string packet,
                         Ptr<Socket> outgoingSocket);

    protected:
        // inherited from Application base class.
        virtual void StartApplication(void); // Called at time specified by Start
        virtual void StopApplication(void); // Called at time specified by Stop

        virtual void DoDispose(void);

        /**
         * \brief Handle a packet received by the application
         * \param socket the receiving socket
         */
        void HandleRead(Ptr<Socket> socket);

        /**
         * \brief Handle an incoming connection
         * \param socket the incoming connection socket
         * \param from the address the connection is from
         */
        void HandleAccept(Ptr<Socket> socket, const Address &from);

        /**
         * \brief Handle an connection close
         * \param socket the connected socket
         */
        void HandlePeerClose(Ptr<Socket> socket);

        /**
         * \brief Handle an connection error
         * \param socket the connected socket
         */
        void HandlePeerError(Ptr<Socket> socket);

    private:

        // In the case of TCP, each socket accept returns a new socket, so the
        // listening socket is stored separately from the accepted sockets
        Ptr<Socket> m_socket;                    //!< Listening socket
        Address m_local;                              //!< Local address to bind to
        TypeId m_tid;                                 //!< Protocol TypeId
        int m_numberOfPeers;                          //!< Number of node's peers
        int m_gatewayNodeId;             //!< Node Id for the gateway
        Ipv4Address m_gatewayAddress;  //!< Ipv6 address of the gateway
        double m_downloadSpeed;                       //!< The download speed of the node in Bytes/s
        double m_uploadSpeed;                         //!< The upload speed of the node in Bytes/s

        std::vector<Ipv4Address> m_peersAddresses;           //!< The addresses of peers
        std::map<Ipv4Address, double> m_peersDownloadSpeeds;      //!< The peersDownloadSpeeds of channels
        std::map<Ipv4Address, double> m_peersUploadSpeeds;        //!< The peersUploadSpeeds of channels

        std::map<Ipv4Address, Ptr<Socket> > m_peersSockets;            //!< The sockets of peers
        nodeStatistics *m_nodeStats;                //!< struct holding the node stats
        std::map<Ipv4Address, std::vector<std::string> > m_messages; //!< map holding all the messages from different peers
        enum ProtocolType m_protocolType;                                               //!< protocol type

        const int m_commPort;               //!< 5555
        const int m_secondsPerMin;             //!< 60

        /// Traced Callback: received packets, source address.
        TracedCallback<Ptr<const Packet>, const Address &> m_rxTrace;


    };


} // namespace ns3

