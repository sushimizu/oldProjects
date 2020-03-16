#!/usr/bin/python
import cgi
import os
import random
import pickle

guess = None
output = ""

#read and return room's resources
def readResources():
	f = open('resources.csv', 'rb')
	line = f.readline()
	mannaR, goldR, _ = map(int, line.split(','))
	f.close()
	return mannaR, goldR

#read and return player's inventory
def readInv():
	f1 = open('inventory.csv', 'rb')
	line = f1.readline()
	mannaI, goldI = map(int, line.split(','))
	f1.close()
	return mannaI, goldI

#writes input to player's inventory
def writeInv(mannaI,goldI):
	f1 = open('inventory.csv', 'wb')
	f1.write("%d,%d" %(mannaI, goldI))
	f1.close()
	
#writes input to room's resources
def writeResources(mannaR,goldR):
	f2 = open('resources.csv', 'wb')
	f2.write("%d,%d,1"%( mannaR, goldR))
	f2.close()
	

def invalid():
	output = 'Incorrect input: must be an integer between 0 and 1024'

#generating random number at the start of each game 
def generate():	
	NUMBER = random.randint(0,1024)
	data = { 'number': NUMBER, 'guesses': 10, 'win': 0 }
	f = open('NUMBER.txt' , 'wb')
	pickle.dump(data,f)	
	f.close()
	output='Guess number '

#correcting resources and inventory after player chooses prize after win
def winScreen(manna , gold):
	#read inventory
	mannaI = readInv()[0]
	goldI = readInv()[1]

	#read resources
	mannaR = readResources()[0]
	goldR = readResources()[1]
	
	mannaI = mannaI + manna
	goldI = goldI + gold 
	mannaR = mannaR - manna
	goldR = goldR - gold

	#replace inventory
	writeInv(mannaI,goldI)

	#replace resources
	writeResources(mannaR,goldR)

	global output
	output = 'Sucess! You gained %d manna and %d gold! <a href="./room.cgi?command=RETURN">return to room </a>'%(manna,gold)


def playTurn(guess):
	f = open('NUMBER.txt', 'rb')
	data = pickle.load(f)
	global output
	#if player wins game
	if guess == data['number']:
		data['win'] = 1
		mannaR = readResources()[0]
		goldR = readResources()[1]
		output  = "Congratulations!  Choose your prize in form: 'manna ,  gold' (where manna + gold = 5). This room contains %d manna and %d gold" %(mannaR, goldR)
	#NUMBER is higher than the guess
	elif  guess < data['number']:
		output = 'HIGHER'
		data['guesses'] = data['guesses'] - 1
	#NUMBER is lower than the guess
	elif  data['number'] < guess:
		output = 'LOWER'
		data['guesses'] = data['guesses']- 1 
	output += ' - Guesses remaining: '+ str(data['guesses'])
	f.close()
	#player runs out of guesses
	if data['guesses'] <= 0 :
		output = 'Sorry, you lose <a href="./room.cgi?command=RETURN">Return to room </a> or  <a href="./game.py"> Play again </a>'
		guess = None
	#keeping track of number of guesses taken
	f = open('NUMBER.txt', 'wb')
	pickle.dump(data,f)
	f.close()
	

try:
	command=os.environ['QUERY_STRING']
	if not command:
		#generating random number NUMBER if starting the game
		generate()
	else:
		f = open('NUMBER.txt','rb')
		data = pickle.load(f)
		if data["win"] == 1:
			#if player wins, read resources of room to tell them what's available 
			mannaR = readResources()[0]
			goldR = readResources()[1]
			args = cgi.FieldStorage()
			#reading the player's input 
			command = args['command'].value
			manna, gold = map(int, command.split(','))
			if 0 <= manna + gold and manna + gold <= 5 and manna <= mannaR and gold <= goldR:
				#valid input shows the winscreen of successful transaction 
				winScreen(manna, gold)
			else:
				output = "Invalid input: Choose your prize in form: 'manna , gold' (where manna + gold = 5). This room contains %d manna and %d gold" %(mannaR, goldR)

		else:
			guess = command[8:]
			try:
				#player may exit game at any point by typing 'QUIT'
				if guess == 'QUIT':
					output = "<a href='./room.cgi?command=RETURN'> Yes, return to room  </a>"
				else:
					guess = int(guess)
					playTurn(guess)
			except Exception as e:
				output = "Please enter a number"
except Exception as e:
	output = 'Internal error: ' + str(e)

#read inventory
mannaI = readInv()[0]
goldI = readInv()[1]

#if player has more than 100 gold, they win the game 
if goldI >= 100:
	print "Content-type: text/html"
	print ""
	print """

	<!doctype html>
	<html>
	<style>
	 h1 { 
		color: gold;
	}
	</style>
	<body>
		<h1><center> CONGRATS! YOU WIN!</h1></center>
		<center> <a href="./room.cgi">Play again</a> </center>
		
	</body>
	</html>
	"""
	mannaR = readResources()[0]
	goldR = readResources()[1]
	f = open('resources.csv', 'wb')
	f.write("%d,%d,0"%( mannaR, goldR))
	f.close()
	

elif goldI < 100:
	print "Content-type: text/html"
	print ""
	print """

	<!doctype html>
	<html>
	<style>
	 h1 { 
		color: green;
	}
	</style>
	<body>
		<h1><center>Let's Play: Higher Lower</center></h1>
		<center><img src="../room.jpg" width="900px" height="500px"/>
		<section>
			<p>You get 10 attempts to guess the random number between 0 and 1024. You will be informed whether the number is higher or lower than your guess. Enter 'QUIT' to return to main page</p>
			<form action="./game.py"  method="get">
				<input type="text" name="command" placeholder="Enter your guess"/>
				<input type="submit" text="asdf"/>
			</form>
			%s
		</section></center>
	</body>
	</html>
	""" % output


