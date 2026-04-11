const { startWpp, sendMessage, logout } = require("./wpp");
const net = require('net')
const fs = require('fs')

const SOCKET_PATH = '/tmp/wpp.sock'

if (fs.existsSync(SOCKET_PATH)) {
  fs.unlinkSync(SOCKET_PATH)
}

let client = null

const server = net.createServer((conn) => {
  
  console.log("C++ conectado")

  client = conn
  conn.on('data', async (data) => {
    const msg = JSON.parse(data.toString())
    console.log('Recebido', msg)
    if (msg.type === 'send') {
      await sendMessage(msg.jid, msg.text)
      
      conn.write(JSON.stringify({ type: 'ok' }) + '\n')
    }else if(msg.type === 'command'){
      if(msg.instruction === 'start' ){
        startWpp(client)
      }else if(msg.instruction === 'logout'){
        logout();
      }
    }
  })

  conn.on('end', () => {
    console.log("C++ desconectado")
    client = null
  })
})

server.listen('/tmp/wpp.sock')
