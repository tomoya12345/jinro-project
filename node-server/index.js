import { PrismaClient } from '@prisma/client';
import express from 'express';
import cors from 'cors';

const prisma = new PrismaClient();
const app = express();
app.use(express.json());
app.use(cors());

// ① 部屋を作るAPI
app.post('/api/room/create', async (req, res) => {
  const { roomNumber, totalPlayers } = req.body;
  const room = await prisma.room.create({
    data: { roomNumber, totalPlayers, phase: 0 }
  });
  res.json(room);
});

// ② プレイヤーが参加するAPI（★今回追加！）
app.post('/api/player/join', async (req, res) => {
  const { name, role, roomNumber } = req.body;
  
  // 部屋番号から部屋を探す
  const room = await prisma.room.findUnique({ where: { roomNumber } });
  
  // プレイヤーをDBに登録する
  const player = await prisma.player.create({
    data: { name, role, roomId: room.id }
  });
  
  res.json(player);
});

const PORT = 3000;
app.listen(PORT, () => console.log(`Node.js Server running on http://localhost:${PORT}`));