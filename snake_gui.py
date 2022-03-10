#!/usr/bin/env python
import sys
sys.path.insert(1, sys.argv[1])
from duels import Subscriber
game = Subscriber(15000)
init_msg = game.get_init()

## Other librairies and set the loading and the path of the file.

import pygame
from pygame.font import FontType
import os

game_path = os.path.abspath(os.path.dirname(__file__)) #Set the path of this file.

def load_image(filename):
    return pygame.image.load('{}/ressource/{}'.format(game_path, filename))

## Get the images of the game, in a dimension 16x16 pixels ===============================

blockImage = load_image("block.png")
appleImage = load_image("apple.png")
meHeadImage =[load_image("player1_head_0.png"),load_image("player1_head_1.png"),load_image("player1_head_2.png"),load_image("player1_head_3.png")]
opponentHeadImage =[load_image("player2_head_{}.png".format(i)) for i in range(4)]
meBodyImage = load_image("player1_body.png")
opponentBodyImage = load_image("player2_body.png")


## Initialisation of the window  =========================================================

imageDimension = 16

numberOfRow = init_msg.width
numberofColumn = init_msg.height

screenWidth = numberOfRow * imageDimension
screenHeight = numberofColumn * imageDimension

pygame.init()

screen = pygame.display.set_mode((screenWidth, screenHeight))
screen.fill((0, 0, 0));


#for row in range(numberOfRow):
#    for column in range(numberofColumn):
#        if row == 0 or row == numberOfRow-1 or column == 0 or column == numberofColumn-1:
#            screen.blit(blockImage, (row*imageDimension, column*imageDimension))

pygame.display.set_caption("Snake Game")
pygame.display.set_icon(meHeadImage[0])

#Getting Snake1 info
meHeadOrientation = init_msg.head1.orientation
meHead_x = init_msg.head1.x
meHead_y = init_msg.head1.y
meBody = init_msg.body1

opponentHeadOrientation = init_msg.head2.orientation
opponentHead_x = init_msg.head2.x
opponentHead_y = init_msg.head2.y
opponentBody = init_msg.body2

apples = init_msg.apples

pygame.display.flip()
game.ready()

## Utilitary Functions ===================================================================

def blitSnakeHead(headOrientation, xHead, yHead, image):
    screen.blit(image[headOrientation], (xHead*imageDimension, yHead*imageDimension))

def blitOneImage(list, image):
    for element in list:
        screen.blit(image, (element.x*imageDimension, element.y*imageDimension))

## Main Loop =============================================================================

while True:
    msg = game.refresh()
    if game.winner:
        break
    # update display from msg fields
    screen.fill((0,0,0))

    ## Getting the new data
    meHeadOrientation = msg.head1.orientation
    meHead_x = msg.head1.x
    meHead_y = msg.head1.y
    meBody = msg.body1

    opponentHeadOrientation = msg.head2.orientation
    opponentHead_x = msg.head2.x
    opponentHead_y = msg.head2.y
    opponentBody = msg.body2

    apples = msg.apples

    ## Displaying the data
    blitSnakeHead(meHeadOrientation, meHead_x, meHead_y, meHeadImage)
    blitSnakeHead(opponentHeadOrientation, opponentHead_x, opponentHead_y, opponentHeadImage)

    blitOneImage(meBody, meBodyImage)
    blitOneImage(opponentBody, opponentBodyImage)

    blitOneImage(apples, appleImage)

    pygame.display.update()


# update display from game.winner (1 or 2, 3 for draw or -1 if any bug that got you there)
# if winner is 1 or 2 you can use game.winner_name(init_msg) to get their name

pygame.display.flip()

pygame.quit()











