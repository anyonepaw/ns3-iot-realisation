//
// Created by anyonepaw on 2019-04-19.
//

#ifndef NS3_IOT_REALIZATION_SNIFFERNODE_H
#define NS3_IOT_REALIZATION_SNIFFER_NODE_H

namespace  ns3{

    class Sniffer : public Application {





        const int m_commPort;               //!< 5555
        const int m_secondsPerMin;             //!< 60

    };

}


#endif //NS3_IOT_REALIZATION_SNIFFIERNODE_H
