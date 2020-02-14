# Minecraft Turtle Example - Crazy Pattern
from mcturtle import minecraftturtle
from mcpi import minecraft
from mcpi import block

# create connection to minecraft
mc = minecraft.Minecraft.create()

# get players position
pos = mc.player.getPos()

# create minecraft turtle
steve = minecraftturtle.MinecraftTurtle(mc, pos)

steve.penblock(block.WOOL.id, 11)
steve.speed(10)

for step in range(0, 50):
    steve.forward(50)
    steve.right(123)
