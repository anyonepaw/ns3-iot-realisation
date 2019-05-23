//
// Created by ricardo on 22.04.19.
//






#include <ns3/application.h>
#include <ns3/type-id.h>

namespace ns3 {

    class Address;

    class Socket;

    class Packet;

    class IotNode : public Application {

    public:

        //----------
        static TypeId GetTypeId(void);

        IotNode();

        virtual ~IotNode();

        Ptr<Socket> GetListeningSocket(void) const;

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

        void SetPeersUploadSpeeds(const std::map<Ipv4Address, double> &peersUploadSpeeds);



        //------------

        void GetNodeId();

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

        std::vector<Ipv4Address> m_peersAddresses;           //!< The addresses of peers
        std::map<Ipv4Address, double> m_peersDownloadSpeeds;      //!< The peersDownloadSpeeds of channels
        std::map<Ipv4Address, double> m_peersUploadSpeeds;        //!< The peersUploadSpeeds of channels

        const int m_commPort;               //!< 5555
        const int m_secondsPerMin;             //!< 60

        std::map<Ipv4Address, Ptr<Socket> > m_peersSockets;            //!< The sockets of peers




        TracedCallback<Ptr<const Packet>, const Address &> m_rxTrace;
    };
}

