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

async function logout(){
  await sock.logout()
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
    browser: ['Chrome (Linux)', 'Chrome', '120.0.0'],
    syncFullHistory: true
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
      sendToCpp({type: 'login', data: 'SUCCESS'})
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
    for (const msg of messages) {
        console.log("Mensagem Nova:", msg)
    }
  })

  sock.ev.on('messaging-history.set', ({ chats, messages, contacts }) => {
    const chatsSerialized = []
    const messagesSerialized = []
    const contactsSerialized = []

    for (const chat of chats) {
      let participants = []
      let isGroup = false

      if (chat.id?.endsWith('@s.whatsapp.net')) {
        participants = [chat.id]
      }
      else if (chat.id?.endsWith('@g.us')) {
        isGroup = true
        participants =
          (chat.participants || [])
            .map(p => p.id || p.participant || '')
            .filter(Boolean)
      }

      chatsSerialized.push({
        id: chat.id,
        isArchived: chat.archived,
        participants,
        isGroup
      })
    }

    for (const message of messages) {
      messagesSerialized.push({
        id: message.key?.id || '',
        contactId: message.key?.remoteJid || '',
        message:
          message.message?.conversation ||
          message.message?.extendedTextMessage?.text ||
          ''
      })
    }

    for (const contact of contacts) {
      contactsSerialized.push({
        id: contact.id,
        name: contact.name,
        phone: contact.phoneNumber
      })
    }

    sendToCpp({ type: 'chats', data: chatsSerialized })
    sendToCpp({ type: 'historic_messages', data: messagesSerialized })
    sendToCpp({ type: 'contacts', data: contactsSerialized })
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
    logout,
    sendToCpp
}