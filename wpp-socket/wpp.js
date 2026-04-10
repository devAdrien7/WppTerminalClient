const {
  default: makeWASocket,
  //useMultiFileAuthState,
  DisconnectReason
} = require('@whiskeysockets/baileys')
const QRCode = require('qrcode')
const keytar = require('keytar')
const { initAuthCreds, BufferJSON } = require('@whiskeysockets/baileys')
 
let sock
let isConnected = false

const SERVICE = 'wpp-app'
const ACCOUNT = 'session'

async function loadAuthState() {
  const data = await keytar.getPassword(SERVICE, ACCOUNT)

  if (!data) {
    return {
      creds: initAuthCreds(),
      keys: {
        get: async () => ({}),
        set: async () => {}
      }
    }
  }

  const parsed = JSON.parse(data, BufferJSON.reviver)

  return {
    creds: parsed.creds,
    keys: {
      get: async (type, ids) => {
        const result = {}
        for (const id of ids) {
          result[id] = parsed.keys?.[type]?.[id]
        }
        return result
      },
      set: async (data) => {
        for (const type in data) {
          for (const id in data[type]) {
            parsed.keys = parsed.keys || {}
            parsed.keys[type] = parsed.keys[type] || {}
            parsed.keys[type][id] = data[type][id]
          }
        }

        await keytar.setPassword(SERVICE, ACCOUNT, JSON.stringify(parsed, BufferJSON.replacer))
      }
    }
  }
}

async function saveCreds(state) {
  await keytar.setPassword(
    SERVICE,
    ACCOUNT,
    JSON.stringify(state, BufferJSON.replacer)
  )
}

async function deleteSession(){
  await keytar.deletePassword(SERVICE, ACCOUNT)
}

let client = null
function sendToCpp(obj) {
  if (!client) return
  client.write(JSON.stringify(obj) + '\n')
}

async function startWpp(conn) {
  client = conn
  const state = await loadAuthState()
  const { fetchLatestBaileysVersion } = require('@whiskeysockets/baileys')
   const { version } = await fetchLatestBaileysVersion()

  sock = makeWASocket({
    auth: state,
    version,
    browser: ['Chrome (Linux)', 'Chrome', '120.0.0']
  })
 
  sock.ev.on('creds.update', async () => {
    await saveCreds(state)
  })

  //sock.ev.on('creds.update', saveCreds)
 
  sock.ev.on('connection.update', async ({ connection, lastDisconnect, qr }) => {
    if (qr) {
      sendToCpp({type: 'qr',data: qr})
    }

    if (connection === 'open') {
      isConnected = true
    }

    if (connection === 'close') {
      isConnected = false

      const statusCode = lastDisconnect?.error?.output?.statusCode
      const reconectar = statusCode !== DisconnectReason.loggedOut

      if (!reconectar) {
        deleteSession()
      }

      setTimeout(() => {
        startWpp(client)
      }, 3000)
    }
  })
 
  sock.ev.on('messages.upsert', async ({ messages, type }) => {
    if (type !== 'notify') return
 
    const msg = messages[0]
    if (!msg.message) return
    if (msg.key.fromMe) return
 
    const jid = msg.key.remoteJid
    const texto = msg.message.conversation || msg.message.extendedTextMessage?.text
 
    console.log('Message received from:', jid)
    console.log('Content:', texto)
  })
}

async function sendMessage(jid, text){
    if (!isConnected) {
      throw new Error("Not Connected to Wpp");
    }

    await sock.sendMessage(jid, {
        text: text
    })
}
 
module.exports = {
    startWpp,
    sendMessage,
    deleteSession,
    sendToCpp
}