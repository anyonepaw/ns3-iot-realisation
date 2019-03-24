/**
 * This file contains the definitions of the functions declared in blockchain.h
 */


#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/address.h"
#include "ns3/log.h"
#include "blockchain.h"

namespace ns3 {


/**
 *
 * Class Block functions
 *
 */
    /**
    *  TODO:Represent public key data as merkle tree structure.
    */
    Block::Block(int blockHeight, int minerId, int parentBlockMinerId, int blockSizeBytes,
                 double timeCreated, std::map<int, blockDataTuple> blockDataMap, double timeReceived, Ipv4Address receivedFromIpv4Address)
    {
        m_blockHeight = blockHeight;
        m_minerId = minerId;
        m_parentBlockMinerId = parentBlockMinerId;
        m_blockSizeBytes = blockSizeBytes;
        m_timeCreated = timeCreated;
        m_timeReceived = timeReceived;
        m_receivedFromIpv4Address = receivedFromIpv4Address;
        m_blockDataMap = blockDataMap;


    }

    Block::Block()
    {
        blockDataTuple blankBlockData = {0, 0, Ipv4Address("0::0::0::0"), "" ,""};
        std::map<int, blockDataTuple> dataMap;
        dataMap[0] = blankBlockData;
        Block(0, 0, 0, 0, 0, dataMap, 0, Ipv4Address("0::0::0::0"));

    }

    Block::Block (const Block &blockSource)
    {
        m_blockHeight = blockSource.m_blockHeight;
        m_minerId = blockSource.m_minerId;
        m_parentBlockMinerId = blockSource.m_parentBlockMinerId;
        m_blockSizeBytes = blockSource.m_blockSizeBytes;
        m_timeCreated = blockSource.m_timeCreated;
        m_timeReceived = blockSource.m_timeReceived;
        m_receivedFromIpv4Address = blockSource.m_receivedFromIpv4Address;
        m_blockDataMap = blockSource.m_blockDataMap;

    }

    Block::~Block (void)
    {
    }

    int
    Block::GetBlockHeight (void) const
    {
        return m_blockHeight;
    }

    void
    Block::SetBlockHeight (int blockHeight)
    {
        m_blockHeight = blockHeight;
    }

    int
    Block::GetMinerId (void) const
    {
        return m_minerId;
    }

    void
    Block::SetMinerId (int minerId)
    {
        m_minerId = minerId;
    }

    int
    Block::GetParentBlockMinerId (void) const
    {
        return m_parentBlockMinerId;
    }

    void
    Block::SetParentBlockMinerId (int parentBlockMinerId)
    {
        m_parentBlockMinerId = parentBlockMinerId;
    }

    int
    Block::GetBlockSizeBytes (void) const
    {
        return m_blockSizeBytes;
    }

    void
    Block::SetBlockSizeBytes (int blockSizeBytes)
    {
        m_blockSizeBytes = blockSizeBytes;
    }

    double
    Block::GetTimeCreated (void) const
    {
        return m_timeCreated;
    }

    double
    Block::GetTimeReceived (void) const
    {
        return m_timeReceived;
    }


    Ipv4Address
    Block::GetReceivedFromIpv4Address (void) const
    {
        return m_receivedFromIpv4Address;
    }

    void
    Block::SetReceivedFromIpv4Address (Ipv4Address receivedFromIpv4Address)
    {
        m_receivedFromIpv4Address = receivedFromIpv4Address;
    }

    blockDataTuple Block::GetNodeData(int nodeId){
        blockDataTuple dataTuple = m_blockDataMap[nodeId];
        return dataTuple;
    }

    void Block::SetNodeData(int nodeId, blockDataTuple blockData){
        m_blockDataMap[nodeId] = blockData;
    }

    bool
    Block::IsParent(const Block &block) const
    {
        if (GetBlockHeight() == block.GetBlockHeight() - 1 && GetMinerId() == block.GetParentBlockMinerId())
            return true;
        else
            return false;
    }

    bool
    Block::IsChild(const Block &block) const
    {
        if (GetBlockHeight() == block.GetBlockHeight() + 1 && GetParentBlockMinerId() == block.GetMinerId())
            return true;
        else
            return false;
    }


    Block&
    Block::operator= (const Block &blockSource)
    {
        m_blockHeight = blockSource.m_blockHeight;
        m_minerId = blockSource.m_minerId;
        m_parentBlockMinerId = blockSource.m_parentBlockMinerId;
        m_blockSizeBytes = blockSource.m_blockSizeBytes;
        m_timeCreated = blockSource.m_timeCreated;
        m_timeReceived = blockSource.m_timeReceived;
        m_receivedFromIpv4Address = blockSource.m_receivedFromIpv4Address;
        // m_nodeId = blockSource.m_nodeId;
        // m_nodePublicKey = blockSource.m_nodePublicKey;
        // m_signature = blockSource.m_signature;

        return *this;
    }


/**
 *
 * Class Blockchain functions
 *
 */

    Blockchain::Blockchain(void)
    {
        m_noStaleBlocks = 0;
        m_totalBlocks = 0;
        blockDataTuple blankBlockData = {0, 0, Ipv4Address("0::0::0::0"), "" ,""};
        std::map<int, blockDataTuple> dataMap;
        dataMap[0] = blankBlockData;
        Block genesisBlock(0, -1, -2, 0, 0, dataMap, 0, Ipv4Address("0::0::0::0"));
        AddBlock(genesisBlock);
    }

    Blockchain::~Blockchain (void)
    {
    }

    int
    Blockchain::GetNoStaleBlocks (void) const
    {
        return m_noStaleBlocks;
    }


    int
    Blockchain::GetNoOrphans (void) const
    {
        return m_orphans.size();
    }

    int
    Blockchain::GetTotalBlocks (void) const
    {
        return m_totalBlocks;
    }


    int
    Blockchain::GetBlockchainHeight (void) const
    {
        return GetCurrentTopBlock()->GetBlockHeight();
    }

    bool
    Blockchain::HasBlock (const Block &newBlock) const
    {

        if (newBlock.GetBlockHeight() > GetCurrentTopBlock()->GetBlockHeight())
        {
            /* The new block has a new blockHeight, so we haven't received it previously. */

            return false;
        }
        else
        {
            /*  The new block doesn't have a new blockHeight, so we have to check it is new or if we have already received it. */

            for (auto const &block: m_blocks[newBlock.GetBlockHeight()])
            {
                if (block == newBlock)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool
    Blockchain::HasBlock (int height, int minerId) const
    {

        if (height > GetCurrentTopBlock()->GetBlockHeight())
        {
            /* The new block has a new blockHeight, so we haven't received it previously. */

            return false;
        }
        else
        {
            /*  The new block doesn't have a new blockHeight, so we have to check it is new or if we have already received it. */

            for (auto const &block: m_blocks[height])
            {
                if (block.GetBlockHeight() == height && block.GetMinerId() == minerId)
                {
                    return true;
                }
            }
        }
        return false;
    }


// std::string
// Blockchain::GetPublickey (int nodeId)
// {
//   std::map<int, std::string>::const_iterator it = m_public_key_map.find(nodeId);
//   if(it != m_public_key_map.end())
//     return it->second;
//
//   return "";
// }
//
//
// Block
// Blockchain::GetPublickeyBlock (int nodeId)
// {
//   std::map<int, Block >::const_iterator it = m_block_map.find(nodeId);
//   if(it == m_block_map.end())
//       return it->second;
//   return Block(-1, -1, -1, -1, -1, -1, "", "", -1, Ipv4Address("0::0::0::0"));
//   //return nullptr;
//
// }
//
//
// std::string
// Blockchain::GetNodePublicKeySignature (int nodeId)
// {
//     Block block = GetPublickeyBlock(nodeId);
//     return block.GetNodePublicKeySignature();
// }
//
// bool
// Blockchain::CheckPublicKeySignature (int nodeId)\
// {
//     Block block = GetPublickeyBlock(nodeId);
//     return true;
// }


    Block
    Blockchain::ReturnBlock(int height, int minerId)
    {
        std::vector<Block>::iterator  block_it;

        if (height <= GetBlockchainHeight() && height >= 0)
        {
            for (block_it = m_blocks[height].begin();  block_it < m_blocks[height].end(); block_it++)
            {
                if (block_it->GetBlockHeight() == height && block_it->GetMinerId() == minerId)
                    return *block_it;
            }
        }

        for (block_it = m_orphans.begin();  block_it < m_orphans.end(); block_it++)
        {
            if (block_it->GetBlockHeight() == height && block_it->GetMinerId() == minerId)
                return *block_it;
        }

        blockDataTuple blankBlockData = {-1, -1, Ipv4Address("0::0::0::0"), "" ,""};
        std::map<int, blockDataTuple> dataMap;
        dataMap[0] = blankBlockData;
        return Block(-1, -1, -1, -1, -1, dataMap, -1, Ipv4Address("0::0::0::0"));
    }


    bool
    Blockchain::IsOrphan (const Block &newBlock) const
    {
        for (auto const &block: m_orphans)
        {
            if (block == newBlock)
            {
                return true;
            }
        }
        return false;
    }


    bool
    Blockchain::IsOrphan (int height, int minerId) const
    {
        for (auto const &block: m_orphans)
        {
            if (block.GetBlockHeight() == height && block.GetMinerId() == minerId)
            {
                return true;
            }
        }
        return false;
    }


    const Block*
    Blockchain::GetBlockPointer (const Block &newBlock) const
    {

        for (auto const &block: m_blocks[newBlock.GetBlockHeight()])
        {
            if (block == newBlock)
            {
                return &block;
            }
        }
        return nullptr;
    }

    const std::vector<const Block *>
    Blockchain::GetChildrenPointers (const Block &block)
    {
        std::vector<const Block *> children;
        std::vector<Block>::iterator  block_it;
        int childrenHeight = block.GetBlockHeight() + 1;

        if (childrenHeight > GetBlockchainHeight())
            return children;

        for (block_it = m_blocks[childrenHeight].begin();  block_it < m_blocks[childrenHeight].end(); block_it++)
        {
            if (block.IsParent(*block_it))
            {
                children.push_back(&(*block_it));
            }
        }
        return children;
    }


    const std::vector<const Block *>
    Blockchain::GetOrphanChildrenPointers (const Block &newBlock)
    {
        std::vector<const Block *> children;
        std::vector<Block>::iterator  block_it;

        for (block_it = m_orphans.begin();  block_it < m_orphans.end(); block_it++)
        {
            if (newBlock.IsParent(*block_it))
            {
                children.push_back(&(*block_it));
            }
        }
        return children;
    }


    const Block*
    Blockchain::GetParent (const Block &block)
    {
        std::vector<Block>::iterator  block_it;
        int parentHeight = block.GetBlockHeight() - 1;

        if (parentHeight > GetBlockchainHeight() || parentHeight < 0)
            return nullptr;

        for (block_it = m_blocks[parentHeight].begin();  block_it < m_blocks[parentHeight].end(); block_it++)  {
            if (block.IsChild(*block_it))
            {
                return &(*block_it);
            }
        }

        return nullptr;
    }


    const Block*
    Blockchain::GetCurrentTopBlock (void) const
    {
        return &m_blocks[m_blocks.size() - 1][0];
    }


    void
    Blockchain::AddBlock (const Block& newBlock)
    {
        if (m_blocks.size() == 0)
        {
            std::vector<Block> newHeight(1, newBlock);
            m_blocks.push_back(newHeight);
        }
        else if (newBlock.GetBlockHeight() > GetCurrentTopBlock()->GetBlockHeight())
        {
            /**
             * The new block has a new blockHeight, so have to create a new vector (row)
             * If we receive an orphan block we have to create the dummy rows for the missing blocks as well
             */
            int dummyRows = newBlock.GetBlockHeight() - GetCurrentTopBlock()->GetBlockHeight() - 1;

            for(int i = 0; i < dummyRows; i++)
            {
                std::vector<Block> newHeight;
                m_blocks.push_back(newHeight);
            }

            std::vector<Block> newHeight(1, newBlock);
            m_blocks.push_back(newHeight);
        }
        else
        {
            /* The new block doesn't have a new blockHeight, so we have to add it in an existing row */

            if (m_blocks[newBlock.GetBlockHeight()].size() > 0)
                m_noStaleBlocks++;

            m_blocks[newBlock.GetBlockHeight()].push_back(newBlock);
        }
        m_totalBlocks++;
    }


    void
    Blockchain::AddOrphan (const Block& newBlock)
    {
        m_orphans.push_back(newBlock);
    }


    void
    Blockchain::RemoveOrphan (const Block& newBlock)
    {
        std::vector<Block>::iterator  block_it;

        for (block_it = m_orphans.begin();  block_it < m_orphans.end(); block_it++)
        {
            if (newBlock == *block_it)
                break;
        }

        if (block_it == m_orphans.end())
        {
            // name not in vector
            return;
        }
        else
        {
            m_orphans.erase(block_it);
        }
    }


    void
    Blockchain::PrintOrphans (void)
    {
        std::vector<Block>::iterator  block_it;

        std::cout << "The orphans are:\n";

        for (block_it = m_orphans.begin();  block_it < m_orphans.end(); block_it++)
        {
            std::cout << *block_it << "\n";
        }

        std::cout << "\n";
    }


    int
    Blockchain::GetBlocksInForks (void)
    {
        std::vector< std::vector<Block>>::iterator blockHeight_it;
        int count = 0;

        for (blockHeight_it = m_blocks.begin(); blockHeight_it < m_blocks.end(); blockHeight_it++)
        {
            if (blockHeight_it->size() > 1)
                count += blockHeight_it->size();
        }

        return count;
    }


    int
    Blockchain::GetLongestForkSize (void)
    {
        std::vector< std::vector<Block>>::iterator   blockHeight_it;
        std::vector<Block>::iterator                 block_it;
        std::map<int, int>                           forkedBlocksParentId;
        std::vector<int>                             newForks;
        int maxSize = 0;

        for (blockHeight_it = m_blocks.begin(); blockHeight_it < m_blocks.end(); blockHeight_it++)
        {

            if (blockHeight_it->size() > 1 && forkedBlocksParentId.size() == 0)
            {
                for (block_it = blockHeight_it->begin();  block_it < blockHeight_it->end(); block_it++)
                {
                    forkedBlocksParentId[block_it->GetMinerId()] = 1;
                }
            }
            else if (blockHeight_it->size() > 1)
            {
                for (block_it = blockHeight_it->begin();  block_it < blockHeight_it->end(); block_it++)
                {
                    std::map<int, int>::iterator mapIndex = forkedBlocksParentId.find(block_it->GetParentBlockMinerId());

                    if(mapIndex != forkedBlocksParentId.end())
                    {
                        forkedBlocksParentId[block_it->GetMinerId()] = mapIndex->second + 1;
                        if(block_it->GetMinerId() != mapIndex->first)
                            forkedBlocksParentId.erase(mapIndex);
                        newForks.push_back(block_it->GetMinerId());
                    }
                    else
                    {
                        forkedBlocksParentId[block_it->GetMinerId()] = 1;
                    }
                }

                for (auto &block : forkedBlocksParentId)
                {
                    if (std::find(newForks.begin(), newForks.end(), block.first) == newForks.end() )
                    {
                        if(block.second > maxSize)
                            maxSize = block.second;
                        forkedBlocksParentId.erase(block.first);
                    }
                }
            }
            else if (blockHeight_it->size() == 1 && forkedBlocksParentId.size() > 0)
            {

                for (auto &block : forkedBlocksParentId)
                {
                    if(block.second > maxSize)
                        maxSize = block.second;
                }

                forkedBlocksParentId.clear();
            }
        }

        for (auto &block : forkedBlocksParentId)
        {
            if(block.second > maxSize)
                maxSize = block.second;
        }

        return maxSize;
    }


    bool operator== (const Block &block1, const Block &block2)
    {
        if (block1.GetBlockHeight() == block2.GetBlockHeight() && block1.GetMinerId() == block2.GetMinerId())
            return true;
        else
            return false;
    }

    std::ostream& operator<< (std::ostream &out, const Block &block)
    {

        out << "(m_blockHeight: " << block.GetBlockHeight() << ", " <<
            "m_minerId: " << block.GetMinerId() << ", " <<
            // "m_nodeId: " << block.GetNodeIdOfBlock() << ", " <<
            // "m_nodePublicKey: " << block.GetNodePublicKey() << ", " <<
            // "m_signature: " << block.GetNodePublicKeySignature() << ", " <<
            "m_parentBlockMinerId: " << block.GetParentBlockMinerId() << ", " <<
            "m_blockSizeBytes: " << block.GetBlockSizeBytes() << ", " <<
            "m_timeCreated: " << block.GetTimeCreated() << ", " <<
            "m_timeReceived: " << block.GetTimeReceived() << ", " <<
            "m_receivedFromIpv4Address: " << block.GetReceivedFromIpv4Address() <<
            ")";
        return out;
    }

    std::ostream& operator<< (std::ostream &out, Blockchain &blockchain)
    {

        std::vector< std::vector<Block>>::iterator blockHeight_it;
        std::vector<Block>::iterator  block_it;
        int i;

        for (blockHeight_it = blockchain.m_blocks.begin(), i = 0; blockHeight_it < blockchain.m_blocks.end(); blockHeight_it++, i++)
        {
            out << "  BLOCK HEIGHT " << i << ":\n";
            for (block_it = blockHeight_it->begin();  block_it < blockHeight_it->end(); block_it++)
            {
                out << *block_it << "\n";
            }
        }

        return out;
    }

    const char* getMessageName(enum Messages m)
    {
        switch (m)
        {
            case INV: return "INV";
            case GET_HEADERS: return "GET_HEADERS";
            case HEADERS: return "HEADERS";
            case GET_BLOCKS: return "GET_BLOCKS";
            case BLOCK: return "BLOCK";
            case GET_DATA: return "GET_DATA";
            case NO_MESSAGE: return "NO_MESSAGE";
            case EXT_INV: return "EXT_INV";
            case EXT_GET_HEADERS: return "EXT_GET_HEADERS";
            case EXT_HEADERS: return "EXT_HEADERS";
            case EXT_GET_BLOCKS: return "EXT_GET_BLOCKS";
            case EXT_GET_DATA: return "EXT_GET_DATA";
            case SEND_PUBLIC_KEY: return "SEND_PUBLIC_KEY";
            case RECEIVE_PUBLIC_KEY: return "RECEIVE_PUBLIC_KEY";
            case RECEIVE_MESSAGE: return "RECEIVE_MESSAGE";
        }
    }

    const char* getMinerType(enum MinerType m)
    {
        switch (m)
        {
            case NORMAL_MINER: return "NORMAL_MINER";
            case GATEWAY_MINER: return "GATEWAY_MINER";
            case SIMPLE_ATTACKER: return "SIMPLE_ATTACKER";
            case MALICIOUS_NODE: return "MALICIOUS_NODE";
            case MALICIOUS_NODE_TRIALS: return "MALICIOUS_NODE_TRIALS";
        }
    }

    const char* getBlockBroadcastType(enum BlockBroadcastType m)
    {
        switch (m)
        {
            case STANDARD: return "STANDARD";
            case UNSOLICITED: return "UNSOLICITED";
            case RELAY_NETWORK: return "RELAY_NETWORK";
            case UNSOLICITED_RELAY_NETWORK: return "UNSOLICITED_RELAY_NETWORK";
        }
    }

    const char* getProtocolType(enum ProtocolType m)
    {
        switch (m)
        {
            case STANDARD_PROTOCOL: return "STANDARD_PROTOCOL";
            case SENDHEADERS: return "SENDHEADERS";
        }
    }

    const char* getManufacturerID(enum ManufacturerID m)
    {
        switch (m)
        {
            case SAMSUNG: return "SAMSUNG";
            case QUALCOMM: return "QUALCOMM";
            case CISCO: return "CISCO";
            case ERICSSON: return "ERICSSON";
            case IBM: return "IBM";
            case SILICON: return "SILICON";
            case OTHER: return "OTHER";
        }
    }


    enum ManufacturerID getManufacturerEnum(uint32_t n)
    {
        switch (n)
        {
            case 0: return SAMSUNG;
            case 1: return QUALCOMM;
            case 2: return CISCO;
            case 3: return ERICSSON;
            case 4: return IBM;
            case 5: return SILICON;
            case 6: return OTHER;
        }
    }
}// Namespace ns3