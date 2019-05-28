//
// Created by anyonepaw on 2019-04-19.
//

#include <ns3/type-id.h>
#include <ns3/application.h>
#include <ns3/socket.h>
#include <ns3/node-container.h>
#include <ns3/point-to-point-module.h>


#ifndef NS3_IOT_REALIZATION_SNIFFIERNODE_H
#define NS3_IOT_REALIZATION_SNIFFIER_NODE_H

namespace ns3 {

    class SnifferNode : public Application {
    public:


        static TypeId GetTypeId(void);

        SnifferNode();

        ~SnifferNode() override;

        void AddListeningNode(Address address);

        void SetListeningP2P(const PointToPointHelper& pointToPointHelper);


    protected:

        void StartApplication() override;
        void StopApplication() override;

        void DoDispose() override;


    private:

        Ptr<Socket> m_socket;//!< Listening socket
        Address m_local;                              //!< Local address to bind to
        TypeId m_tid;                                 //!< Protocol TypeId
        TypeId m_attackingNode_tid;                                 //!< Protocol TypeId
        std::vector<Ipv4Address> m_peersAddresses;           //!< The addresses of peers
        Ptr<Node> m_attackingNode;
        PointToPointHelper m_p2p;
    };
}
#endif
