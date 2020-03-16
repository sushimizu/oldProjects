#!/usr/bin/python

import cgi

#read the  input at end of surrent url 
args = cgi.FieldStorage()

#if 1 manna when transporting 
def deathscreen():

	print "Content-type: text/html\n\n"
	print ""
	print """
	<!doctype html>
	<html>
	<style>
	h1{
		color: black;
	}
	</style>
	<body>
	<h1><center> Sorry, no manna left! You lose :( </h1></center>
	<section>
	<center><a href='http://cs.mcgill.ca/~mshimi2/cgi-bin/room.cgi'> Replay </a></center>
	</section>
	</body>
	</html>
	"""

try:
	#separating the arguments from end of current url 
	inventory = args['inventory'].value
	url = args['URL'].value
except KeyError as E:
	pass

#reading the inventory from url info 
mannaI, goldI = map(int,inventory.split(','))
#reading the room's resources
f = open("resources.csv","rb")
rec = f.readline()
mannaR, goldR , oc = map(int, rec.split(',' ))
f.close()
#if the room is occupied, player is sent back to their previous room 
if oc == 1 :
	print "Location: %s\n\n" %url
# if the room is empty
else:
	#we take 1 manna away 
	mannaI = mannaI - 1
	#and check if they have any mannna left, of no, deathscreen and room is empty
	if mannaI == 0 :
		deathscreen()
		f1 = open("resources.csv", "wb")
		f1.write("%d,%d,0" %(mannaR,goldR))
		f1.close()
	#if they have enough manna
	else:
		#the room resources are rewritten, with the room occupied
		f1 = open("resources.csv" ,"wb")
		f1.write("%d,%d,1" % (mannaR,goldR))
		f1.close()
		#and thier inventory is passed to the inventory.csv file for this room to read
		f2 = open("inventory.csv","w")
		f2.write("%d,%d" %(mannaI,goldI))
		f2.close()
		
		#the player is sent to the room 
		print "Location: http://cs.mcgill.ca/~mshimi2/cgi-bin/room.cgi?command=TRANSPORT\n\n" 

