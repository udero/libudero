﻿/**
 * @file CANopen.h
 * @copyright RT Corp. 2012. Allrights Reserved.
 */
#ifndef CANOPEN_HEADER_INCLUDED
#define CANOPEN_HEADER_INCLUDED

#include "CANopen_defines.h"
#include "ICANInterface.h"
#include "Thread.h"
#include "Node.h"
#include "NodeFactory.h"
#include "CANLogger.h"

/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {



	/**
	 * @class CANopen
	 * @if jp
     * @brief CANopen用の基本通信機能および，バックグラウンドジョブによるメッセージ処理を行うクラス
	 * @else
	 * @brief Class for CAN Open protocol
	 * @endif
	 */
	class CANOPEN_API CANopen : public ssr::Thread {

	private:

		/**
		 * @brief 最大ノード数
		 */
		const static uint8_t MAX_NODE = 16;

		ssr::Mutex m_Mutex;

		ssr::Mutex m_NodeListMutex;

		/**
		 * @brief CANバスインターフェース
		 */
		ICANInterface *m_pCAN;
		
		Node* m_ppNode[MAX_NODE];

		bool m_Status; //< Status of CAN interface. True if OK.

		bool m_Endflag; //< Status of Background Thread.

	public:

		/**
		 * @if jp
		 * @brief run処理（Threadクラスのメソッド)
		 * @else
		 * @brief run method derived from Thread class
		 * @endif
		 * @see Thread
		 */
		void Run();
	private:

		/**
		 * @if jp
		 * @brief CANメッセージのNodeクラスへの配布．内部処理に使われる
		 * @param msg 受信したCANメッセージ
		 * @param id_offset この値以上のCOB-ID，かつNode最大数に収まる場合は，後で指定する関数を呼び出してメッセージをNodeに渡す
		 * @param pFunc メッセージをNodeに渡すためのNodeクラスのメンバー関数
		 * @return もし渡すことに成功すればtrueを返すので，dipatch関数を終了する
		 * @else
		 * @brief This function is used in dispatchCANMessage function. 
		 * @param msg Received CAN message
		 * @param id_offset Offset ID 
		 * @param pFunc Node class function to pass CAN messge.
		 * @return true if success, false if failed.
		 * @endif
		 */
		bool dispatchCANMessageSub(const CANMSG& msg, 
						    const uint32_t id_offset,
						    void(Node::*pFunc)(const CANMSG&));
		
		/**
		 * @if jop
		 * @brief CANメッセージの各ノードへの分配処理に関して記述されている
		 * @else
		 * @brief This function is periodically called by run method. 
		 * This function is used by background job to pass CAN mesasge to 
		 * 
		 * @param msg Received CAN message.
		 * @endif
		 */
		void dispatchCANMessage(const CANMSG& msg);

	public:

		/**
		 * @if jp
		 * @brief Nodeの初期化を行う．この処理ではnodeIdで指定されるNodeが検出されると，
		 * registerNodeFactoryメソッドで指定されたINodeFatoryインターフェースを使って製品を識別する．
		 * 具体的には，INodeFactory側から得るベンダーID，プロダクトコード，およびマスクを使い，
		 * NodeのベンダーID，プロダクトコードと照らし合わせてNodeの製品識別し，
		 * INodeFactoryのcreateNodeメソッドを使ってノードを生成し，本クラスに登録を行う．
		 * 
		 * @param nodeId ノードID
		 * @return もし初期化に成功すれば，Nodeへのポインタ．失敗すればnull．
		 * @else
		 * @brief Initialize Node 
		 *
		 * Node specified by the nodeId will initialized. If already initialized, just return the reference pointer.
		 *
		 * @param nodeId Node Identifier.
		 * @return Pointer to Node initialized.
		 * @endif
		 */
		Node* initNode(const uint8_t nodeId);

		/**
		 * @if jp
		 * @brief 登録されているNodeクラスオブジェクトを返す．
		 * @param nodeId ノードID
		 * @return ノードへのポインタ．ノードが初期化されていなければNull
		 * @else
		 * @brief Get CANopen Node
		 * 
		 * @return Pointer to Node object. NULL if the specified Node is not initialized.
		 * @endif
		 * @see initNode
		 */
		Node* getNode(const uint8_t nodeId) throw();

	private:
		/**
		 * @brief Register Node to Node List 
		 *
		 * @param pNode Node to be registered.
		 */
		void registerNode(Node* pNode);

		/**
		 * @brief Unregister Node from Node List. 
		 * WARNING: This function does not destroy Node class Object.
		 *
		 * @param nodeId Node Identifier Number.
		 * @return Pointer to Node unregistered.
		 */
		Node* unregisterNode(const uint8_t nodeId);


	private:

		const static int MAX_NODE_FACTORY = 8;


		INodeFactory* m_ppNodeFactory[MAX_NODE_FACTORY];
	public:

		/**
		 * @brief Register Node Factory Class Object.
		 * Once Factory registered, Node will automatically generated by Factory object.
		 * Factory will be selected using Vendor ID, Product Code, and Product Code Mask.
		 *
		 */
		void registerNodeFactory(INodeFactory* pFactory) {
			for(int i = 0;i < MAX_NODE_FACTORY;i++) {
				if(m_ppNodeFactory[i] == NULL) {
					m_ppNodeFactory[i] = pFactory;
				}
			}
		}

		/**
		 * @brief Search Node Factory This function will be called automatically when Node is initialized.
		 * 
		 * @param vendorId Vendor ID of Node object
		 * @param productId Product ID of Node object
		 */
		INodeFactory* searchNodeFactory(const uint32_t vendorId, const uint32_t productId);



	public:

		/**
		 * Constructor
		 *
		 * @param pCAN CAN Interface object.
		 */
		CANopen(ICANInterface *pCAN);

		/**
		 * destructor
		 */
		virtual ~CANopen();

	public:


		void send(const CANMSG& msg);
		void receive(CANMSG* msg);

	private:
		CANMSG syncMessage;

	public:

		/**
		 * @brief Send Sync Message (PDO protocol)
		 *
		 * PDO protocol message is usually implemented 
		 * in PDOProtocol class.
		 * This function is exceptional implementation. 
		 * Sync Message is used for Synchronizingly Update 
		 * All Nodes' PDO message.
		 * It is preferable to implement to this class.
		 */
		void sendSync(){
			send(syncMessage);
		}

		/**
		 * @brief Send PDO Message (PDO protocol)
		 *
		 * PDO protocol message is usually implemented 
		 * in PDOProtocol class.
		 * This function is basic implementation. 
		 * Sync Message is used for Synchronizingly Update 
		 * All Nodes' PDO message.
		 */
		void sendPDO(const uint8_t nodeId, const PDO_INDEX pdoIndex, const uint8_t length, const uint8_t data[]);

	};

}




#endif
