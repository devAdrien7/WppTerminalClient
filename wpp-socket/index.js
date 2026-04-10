const express = require('express')
const app = express()
const { startWpp, sendMessage, deleteSession } = require("./wpp");

let currentQR = ""
app.use(express.json());

app.get('/', (req, res) => {
  res.send(`
    <html>
      <body>
        <img src="${currentQR}" />
        <script>
          setTimeout(() => location.reload(), 2000)
        </script>
      </body>
    </html>
  `)
})

app.post('/send', async (req, res) => {
    const { number, text } = req.body;

    const jid = number + "@s.whatsapp.net";

    try {
        await sendMessage(jid, text);
        res.send({ status: "ok" });
    } catch (err) {
        res.status(500).send({ error: err.message });
    }
});

deleteSession()
startWpp((qr) => {
  currentQR = qr
})
app.listen(3000)