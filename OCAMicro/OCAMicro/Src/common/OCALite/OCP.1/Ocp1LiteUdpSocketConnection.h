/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteUdpSocketConnection
 *
 */
#ifndef OCP1LITEUDPSOCKETCONNECTION_H
#define OCP1LITEUDPSOCKETCONNECTION_H

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Network/IOcp1LiteSocket.h>
#include <OCC/ControlDataTypes/OcaLiteFrameworkDataTypes.h>
#include <OCF/Messages/OcaLiteMessageGeneral.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "Messages/Ocp1LiteMessageHeader.h"
#include "Ocp1LiteReader.h"
#include "Ocp1LiteWriter.h"

// ---- Referenced classes and types ----
class IOcp1LiteSocket;
class OcaLiteMessageCommand;
class Ocp1LiteUdpSocketConnection;

// ---- Helper types and constants ----
class Ocp1LiteUdpNetwork;

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of a socket connection to a remote controller.
 * Ocp1LiteUdpSocketConnection is responsible for:
 * - sending and receiving Ocp.1Messages
 * - handling KeepAlive messages
 */
class Ocp1LiteUdpSocketConnection
{
public:
    /** Defines the possible return values. */
    enum OcaLiteSocketRetVal
    {
        /** No message is received */
        NO_MESSAGE_RECEIVED,
        /** Received a valid message */
        MESSAGE_RECEIVED,
        /** Lost connection to remote device */
        CONNECTION_LOST,
        /** Connection was corrupted */
        CONNECTION_CORRUPT
    };

    /**
     * Constructor.
     *
     * @param[in]   network         The parent network.
     * @param[in]   bufferSize      The buffer size to use for sending and receiving messages
     */
    Ocp1LiteUdpSocketConnection(::Ocp1LiteUdpNetwork& network,
                             ::OcaUint32 bufferSize);

    /**
     * Destructor.
     */
    virtual ~Ocp1LiteUdpSocketConnection();

    /**
     * Shutdown the SocketConnection.
     */
    void Shutdown();

    /**
     * Send a message via the socket to the remote device
     *
     * @param[in]   msg                     The message to send
     * @param[in]   writer                  The writer to use
     * @param[in]   messageSendBufferSize   Size of the message send buffer
     * @param[in]   pMessageSendBuffer      Arbitrary buffer which can be used marshal data to
     *
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus SendOcaMessage(const ::OcaLiteMessageGeneral* msg,
                                   const ::IOcaLiteWriter& writer,
                                   ::OcaUint32 messageSendBufferSize, 
                                   ::OcaUint8* pMessageSendBuffer);

    /**
     * Function to be called on a regular base.
     *
     * @param[in]  dataAvailable            Indicates that there is data available in the socket.
     * @param[in]  messageSendBufferSize    Size of the message send buffer
     * @param[in]  pMessageSendBuffer       Arbitrary buffer which can be used marshal data to
     * @param[out] bKeepAliveReceived       True if an keepalive is received, false if not.
     * @return  OcaLiteSocketRetVal to indicate action the caller should take
     */
    OcaLiteSocketRetVal Run(::OcaBoolean dataAvailable, 
                            ::OcaUint32 messageSendBufferSize, 
                            ::OcaUint8* pMessageSendBuffer, 
                            ::OcaBoolean& bKeepAliveReceived);

    /**
     * Gets the first pending message. Method can be called until NULL is returned.
     *
     * @param[out] moreComing   True iff more messages must be processed.
     * @return  Pointer to a pending message
     */
    ::OcaLiteMessageGeneral* GetFirstPendingMessage(::OcaBoolean& moreComing);

    /**
     * Assign network socket to the connection socket
     * @param[in]   rSocket   Reference to the UdpNetwork listener socket.
     */
    void InitializeSocket(int rSocket);

    /**
     * Gets the socket belonging to this connection.
     *
     * @return  The socket, NULL if no socket available.
     */
    INT32* GetSocket()
    {
        return &m_socket;
    }

    /**
    * Update the ip address and port of the device
    *
    * @param[in]   ipAddress   ip address of the remote device
    * @param[in]   port        port of the remote device
    */
    void SetRemoteAddressAndPort(const std::string& ipAddress, UINT16 port);

    /**
    * Fill the received data buffer of connection
    *
    * @param[in]   pMessageReceiveBuffer   pointer to the buffer received
    * @param[in]   bytesReceived           size of the received message
    */
    void UpdateReceiveBuffer(OcaUint8* pMessageReceiveBuffer, INT32 bytesReceived);

    /**
    * Gets the ip address of the insecure UDP socket of the remote device for which given socket connection is bound to
    *
    * @return The ip address of the insecure UDP socket of the remote device for which given socket connection is bound to
    */
    const std::string& GetRemoteIpAddress() const
    {
        return m_remoteIpAddress;
    }

    /**
    * Gets the port number of the insecure UDP socket of the remote device for which given socket connection is bound to
    *
    * @return The port number the insecure UDP socket of the remote device for which given socket connection is bound to
    */
    UINT16 GetRemotePort() const
    {
        return m_remotePort;
    }


    /**
     * Gets if socket is connected
     *
     * @return  true if connected, false if not
     */
    bool IsConnected() const
    {
        return (SOCKET_CONNECTED == m_socketState);
    }

    /**
     * Sets the socket connection parameters.
     *
     * @param[in]   sessionID       The session ID for this socket connection
     * @param[in]   timeout         Timeout to use for KeepAlive messages in seconds.
     * @param[in]   keepAliveInMilliseconds indicates if the KeepAlive messages timeout is in seconds or milliseconds
     */
    void SetSocketConnectionParameters(::OcaSessionID sessionID,
                                       ::OcaUint32 timeout,
                                       bool keepAliveInMilliseconds = false);


    /**
     * Are there pending messages for this socket connection?
     *
     * @return True if pending messages, false if not.
     */
    bool HasPendingMessage() const;

    /** Network message delimiter */
    static const ::OcaUint8 MESSAGESYNCVAL;

private:
    /** Defines the possible socket states. */
    enum SocketState
    {
        /** No connection to remote device. */
        SOCKET_NOT_CONNECTED,
        /** Connected to remote device. */
        SOCKET_CONNECTED,
        /** Corrupted connection/message stream. */
        SOCKET_CORRUPT
    };

    /** Enumeration of message states. */
    enum OcaLiteMessageState
    {
        /** State find sync value. */
        OCA_MSG_STATE_SYNC_FIND     = 0,
        /** State read header */
        OCA_MSG_STATE_HEADER_READ   = 1,
        /** State read message. */
        OCA_MSG_STATE_MESSAGE_READ  = 2,
        /** State skip to next message. */
        OCA_MSG_STATE_MESSAGE_SKIP  = 3
    };

    /**
     * Send a KeepAlive message
     *
     * @param[in]   heartBeatTimeOut    The timeout to use
     * @param[in]   messageSendBufferSize   Size of the message send buffer
     * @param[in]   pMessageSendBuffer      Arbitrary buffer which can be used marshal data to
     * @return  OCASTATUS_PROCESSING_FAILED if sending the keepalive failed altogether; OCASTATUS_OK if
     *          sending the keepalive succeeded; OCASTATUS_PARTIALLY_SUCCEEDED or OCASTATUS_BUFFER_OVERFLOW
     *          if sending the keepalive did not succeed due to local issues (socket buffer overflow)
     */
    ::OcaLiteStatus SendKeepAlive(::OcaUint32 heartBeatTimeOut, ::OcaUint32 messageSendBufferSize, 
                                  ::OcaUint8* pMessageSendBuffer);

    /**
     * Handle the keep alive message sending/receiving
     *
     * @param[in]   messageSendBufferSize   Size of the message send buffer
     * @param[in]   pMessageSendBuffer      Arbitrary buffer which can be used marshal data to
     * @return  true if all OK, false if connection is lost
     */
    bool HandleKeepAlive(::OcaUint32 messageSendBufferSize, 
                         ::OcaUint8* pMessageSendBuffer);

    /**
     * Receive message from socket and translate to complete message(s)
     * @param[in]  dataAvailable       Indicates that there is data available in the socket.
     * @param[out] bKeepAliveReceived   True if a keepalive is received, false if not.
     */
    void ReceiveFromSocket(::OcaBoolean dataAvailable, ::OcaBoolean& bKeepAliveReceived);


    /**
     * Reset the message state machine.
     */
    void ResetMessageState();

    /**
     * Update the size of next message (for commands, size of the individual command in bytes)
     */
    void UpdateNextMessageSize();
    

    /** The network this class belongs to. */
    ::Ocp1LiteUdpNetwork&          m_parent;

    /** Session ID */
    ::OcaSessionID              m_sessionID;

    /** The socket to use for the communication */
    ::INT32                     m_socket;

    /** Socket state */
    SocketState                 m_socketState;

    /** Indicates that a keep alive message was received. */
    bool                        m_bKeepAliveReceived;

    /** Indicates that a keep alive time received is in seconds or milliseconds. */
    bool                        m_isKeepAliveInMilliseconds;

    /** Buffer to use for storing received message */
    ::OcaUint8*                 m_pMessageReceiveBuffer;

    /** Size of the message buffers */
    ::OcaUint32                 m_messageBufferSize;

    /** Timestamp to indicate when the last message was sent */
    ::OcaUint32                 m_lastMessageSentTime;

    /** Timestamp to indicate when the last message was received */
    ::OcaUint32                 m_lastMessageReceivedTime;

    /** Total number of bytes received for current message */
    ::OcaUint32                 m_totalLength;

    /** Nr of seconds between keep alive messages */
    ::OcaUint32                 m_keepAliveTimeOut;

    /** Message state */
    OcaLiteMessageState         m_messageState;

    /** Header of the current PDU */
    ::Ocp1LiteMessageHeader*    m_pCurrentHeader;

    /* Bytes left to process in the pdu */
    ::OcaUint32                 m_pduBytesLeft;

    /* Messages left to process in the pdu */
    ::OcaUint16                 m_pduMessagesLeft;

    /* Bytes left to processes in the message */
    ::OcaUint32                 m_messageBytesLeft;

    /** The ipaddress or host name of the remote device/controller */
    std::string                 m_remoteIpAddress;

    /** The port number of the remote device/controller */
    UINT16                      m_remotePort;

    /** private copy constructor, no copying of object allowed */
    Ocp1LiteUdpSocketConnection(const ::Ocp1LiteUdpSocketConnection&);
    /** private assignment operator, no assignment of object allowed */
    ::Ocp1LiteUdpSocketConnection& operator=(const ::Ocp1LiteUdpSocketConnection&);
};

#endif // OCP1LITEUDPSOCKETCONNECTION_H
