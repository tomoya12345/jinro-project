import { PrismaClient } from '@prisma/client';
import express from 'express';
import cors from 'cors';

const prisma = new PrismaClient();
const app = express();
app.use(express.json());
app.use(cors());

// 部屋作成API
app.post('/api/room/create', async (req, res) => {
  const { roomNumber, totalPlayers } = req.body;
  const room = await prisma.room.create({
    data: { roomNumber: roomNumber, totalPlayers: totalPlayers, phase: 0 }
  });
  res.json(room);
});

const PORT = 3000;
app.listen(PORT, () => console.log(`Node.js Server running on http://localhost:${PORT}`));