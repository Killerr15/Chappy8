#include "chip8.hpp"

chip8::chip8() : randGen(std::random_device{}()), dist(0, 255U) { 

	pc = startAddress;

	index = 0;
	sp = 0;
	delayTimer = 0;
	soundTimer = 0;
	opcode = 0;

	memset(v, 0, sizeof(v));
	memset(stack, 0, sizeof(stack));
	memset(ram, 0, sizeof(ram));
	memset(display, 0, sizeof(display));
	memset(keys, 0, sizeof(keys));

	for (unsigned int i = 0; i < fontSize; ++i)
	{
		ram[fontAddress + i] = fontset[i];
	}
};

bool chip8::loadROM(const char* fileName)
{
	std::ifstream file(fileName, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		auto size = file.tellg();
		auto buffer = std::make_unique<char[]>(size);

		if (size > 4096 - 512)
		{
			std::cout << "Error! ROM too big!" << std::endl;
			return 0;
		}
		else
		{
			file.seekg(0, std::ios::beg);
			file.read(buffer.get(), size);
			file.close();

			for (long i = 0; i < size; ++i)
			{
				ram[startAddress + i] = buffer.get()[i];
			}
			std::cout << "ROM loaded." << std::endl;
		}
		return 1;

	}
	std::cout << "Error! Couldn't load ROM file. Make sure you provided correct path to the file." << std::endl;
	return 0;
}

void chip8::emulate()
{
	opcode = (ram[pc] << 8 | ram[pc + 1]);
	pc += 2;

	uint8_t registerX = (opcode & 0x0F00) >> 8;
	uint8_t registerY = (opcode & 0x00F0) >> 4;

	int x, y, ht, wt, pixel, displayIndex;

	bool keyPressed = false;

	std::cout << "Opcode: " << std::hex << opcode << std::endl;


	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x00FF)
		{
		case 0x00E0:
			memset(display, 0, sizeof(display));
			drawFlag = true;
			break;
		case 0x00EE:
			sp--;
			pc = stack[sp];
			break;
		default:
			std::cerr << "Unimplemented opcode!\n";
			break;
		}
		break;
	case 0x1000:
		pc = opcode & 0x0FFF;
		break;
	case 0x2000:
		stack[sp] = pc;
		sp++;
		pc = opcode & 0x0FFF;
		break;
	case 0x3000:
		if (v[registerX] == (opcode & 0x00FF))
		{
			pc += 2;
		}
		break;
	case 0x4000:
		if (v[registerX] != (opcode & 0x00FF))
		{
			pc += 2;
		}
		break;
	case 0x5000:
		if (v[registerX] == v[registerY])
		{
			pc += 2;
		}
		break;
	case 0x6000:
	{
		v[registerX] = opcode & 0x0FFF;
	}
	break;
	case 0x7000:
		v[registerX] += opcode & 0x00FF;
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			v[registerX] = v[registerY];
			break;
		case 0x0001:
			v[registerX] = v[registerX] | v[registerY];
			break;
		case 0x0002:
			v[registerX] = v[registerX] & v[registerY];
			break;
		case 0x0003:
			v[registerX] = v[registerX] ^ v[registerY];
			break;
		case 0x0004:
			v[registerX] += v[registerY];
			if (v[registerX] > 255)
			{
				v[0x0F] = 1;
			}
			else
			{
				v[0x0F] = 0;
			}
			break;
		case 0x0005:
			v[registerX] -= v[registerY];
			break;
		case 0x0006:
			v[0x0F] = v[registerX] & 0x1;
			v[registerX] >>= 1;
			break;
		case 0x0007:
			v[registerX] = v[registerY] - v[registerX];
			break;
		case 0x000E:
			v[0x0F] = v[registerX] >> 7;
			v[registerX] <<= 1;
			break;
		default:
			std::cout << "Unimplemented opcode!\n";
		}
		break;
	case 0x9000:
		if (v[registerX] != v[registerY])
		{
			pc += 2;
		}
		break;
	case 0xA000:
		index = opcode & 0x0FFF;
		break;
	case 0xB000:
		pc = (opcode & 0x0FFF) + v[0];
		break;
	case 0xC000:
		v[registerX] = (opcode & 0x00FF) & dist(randGen);
		break;
	case 0xD000:
		x = v[registerX], y = v[registerY];
		ht = opcode & 0x000F;
		wt = 8;
		v[0x0F] = 0;

		for (int i = 0; i < ht; i++)
		{
			pixel = ram[index + i];
			for (int j = 0; j < wt; j++)
			{
				if ((pixel & (0x80 >> j)) != 0)
				{
					displayIndex = ((x + j) + (y + i) * 64) % 2048;
					if (display[displayIndex] == 1)
					{
						v[0x0F] = 1;
					}
					display[displayIndex] ^= 1;
				}
			}
		}

		drawFlag = true;
		break;
	case 0xE000:
		switch (opcode & 0x00FF)
		{
		case 0x009E:
			if (keys[v[registerX]] != 0)
			{
				pc += 2;
			}
			break;
		case 0x00A1:
			if (keys[v[registerX]] == 0)
			{
				pc += 2;
			}
			break;
		default:
			std::cout << "Unimplemented opcode!\n";
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			v[registerX] = delayTimer;
			break;
		case 0x0015:
			delayTimer = v[registerX];
			break;
		case 0x0018:
			soundTimer = v[registerX];
			break;
		case 0x001E:
			index += v[registerX];
			if (index > 0xFFF)
			{
				v[0x0F] = 1;
			}
			else
			{
				v[0x0F] = 0;
			}
			break;
		case 0x000A:
			for (int i = 0; i < 16; i++)
			{
				if (keys[i] != 0)
				{
					keyPressed = true;
					v[registerX] = (uint8_t) i;
				}
			}

			if (!keyPressed)
			{
				pc -= 2;
			}
			break;
		case 0x0029:
			index = v[registerX] * 0x5;
			break;
		case 0x0033:
			ram[index] = (uint8_t)((uint8_t)v[registerX] / 100);
			ram[index + 1] = (uint8_t)((uint8_t)(v[registerX] / 10) % 10);
			ram[index + 2] = (uint8_t)((uint8_t)(v[registerX] % 100) % 10);
			break;
		case 0x0055:
			for (int i = 0; i <= registerX; i++)
			{
				ram[index + i] = v[i];
			}
			index = index + registerX + 1;
			break;
		case 0x0065:
			for (int i = 0; i <= registerX; i++)
			{
				v[i] = ram[index + i];
			}
			index = index + registerX + 1;
			break;
		default:
			std::cout << "Unimplemented opcode!\n";
		}
		break;
	default:
		std::cerr << "Unimplemented opcode!\n";
		break;
	}

	updateTimers();
}

void chip8::updateTimers()
{
	if (delayTimer > 0)
	{
		delayTimer--;
	}

	if (soundTimer > 0)
	{
		soundTimer--;
		soundFlag = true;
	}
	else
	{
		soundFlag = false;
	}
}

bool chip8::getDrawFlag()
{
	return drawFlag;
}

void chip8::setDrawFlag(bool flag)
{
	drawFlag = flag;
}

bool chip8::getSoundFlag()
{
	return soundFlag;
}

int chip8::getDisplayVal(int i)
{
	return display[i];
}

void chip8::setKeyValue(int x, int y)
{
	keys[x] = y;
}