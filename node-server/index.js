import { PrismaClient } from '@prisma/client';
import express from 'express';
import cors from 'cors';

const prisma = new PrismaClient();
const app = express();

app.use(express.json()); // サイトから届いたJSONデータ（文字）を読みやすくするおまじない
app.use(cors());         // 違うURLからのアクセスを許可するおまじない

// ==========================================================
// 【お手本】部屋を作る窓口 (API)
// Webサイトで「部屋を作る」ボタンが押されたらここが実行されます
// ==========================================================
app.post('/api/room/create', async (req, res) => {
  // 1. サイトから届いたデータ（部屋番号と参加人数）を受け取る
  const { roomNumber, totalPlayers } = req.body;

  // 2. データベースに新しい部屋を保存する
  const room = await prisma.room.create({
    data: { 
      roomNumber: roomNumber, 
      totalPlayers: totalPlayers, 
      phase: 0 // 最初は待機中(0)からスタート
    }
  });

  // 3. サイト側に「無事に部屋ができたよ！」とデータを返してあげる
  res.json(room);
});

// ==========================================================
// 💡 JS班の1年生への宿題・改造指針
// ==========================================================
// 【やってみよう！】プレイヤーが参加するための窓口を作ってみよう！
// 下のコードの「// ここに〜」の部分を書き換えてみてください。
app.post('/api/player/join', async (req, res) => {
  // ① サイトから req.body で「名前(name)」と「部屋番号(roomNumber)」を受け取ろう！
  
  // ② prisma.room.findUnique を使って、その部屋番号のデータをデータベースから探そう！
  
  // ③ prisma.player.create を使って、プレイヤーを登録しよう！
  
  // ④ res.json() で登録できたプレイヤーのデータをサイトに返してあげよう！
});

// サーバーを起動して、アクセスを待ち受ける
const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Node.jsサーバーが http://localhost:${PORT} で動き出しました！`);
});