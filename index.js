const express = require("express");
const app = express();
const cors = require("cors");
const multer = require("multer");
const uuid = require("uuid");

app.use(cors({
    origin: '*'
}));
app.use(express.json({ limit: '100mb' }));
app.use(express.urlencoded({ limit: '100mb', extended: false }));

const storage = multer.diskStorage({
            destination: (req, file, callback) => callback(null, 'Files/'),
            filename: (req, file, callback) => callback(null, `${file.originalname}_${uuid.v4().split('-').join('').toUpperCase()}.${file.mimetype.includes('video') ? `mp4` : file.mimetype.includes(`audio`) ? `mp3` : file.mimetype.split('/')[1]}`)
});

const upload = multer({ storage });

app.get('*', (req, res, next) => {
    const rf = req.originalUrl;
    rf === '/' ? res.sendFile(__dirname + `/index.html`) : rf === '/style.css' ? res.sendFile(__dirname + `/style.css`) : next();
});

app.post('/api', upload.single('file'), (req, res) => {
    req.file ? res.send({ success: true }) : res.sendStatus(400);
});

app.listen(3000, () => console.log('Listening on port 3000...'));