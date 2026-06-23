import { PrismaClient } from '@prisma/client';
import express from 'express';
import cors from 'cors';

const prisma = new PrismaClient();
const app = express();
app.use(express.json());
app.use(cors());

// 【完成済みのお手本】部屋を作成するAPI
app.post('/api/room/create', async (req, res) => {
  const { roomNumber, totalPlayers } = req.body;
  const room = await prisma.room.create({
    data: { roomNumber, totalPlayers, phase: 0 }
  });
  res.json(room);
});

// ==========================================
// 💡 Node.js班 1年生への開発タスク
// ==========================================

// タスク1: プレイヤーが部屋に参加するAPIを作って！
app.post('/api/player/join', async (req, res) => {
  // ① req.body から name と roomNumber を受け取る
  // ② prisma.room.findUnique で部屋の id を探す
  // ③ prisma.player.create でプレイヤーをDBに登録する
  // ④ 登録したプレイヤー情報を res.json() で返す
});

// タスク2: プレイヤーが投票するAPIを作って！
app.post('/api/vote', async (req, res) => {
  // ここに投票データを保存する処理を書く（※後でVoteテーブルも作ろう）
});

const PORT = 3000;
app.listen(PORT, () => console.log(`Node.js Server running on http://localhost:${PORT}`));