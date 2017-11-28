
# coding: utf-8

# In[1]:

# Name Surname, email: Gonul AYCI, gonul.ayci@boun.edu.tr
# Location, Date: Hipo, Nov. 28, 2017
# Description: This document is prepared for Python Hour 


# # Jupyter Notebook 
# http://jupyter.org/ <br>
# Examples: https://nbviewer.jupyter.org/ <br>
# A list of available kernels: https://github.com/jupyter/jupyter/wiki/Jupyter-kernels

# In[2]:

get_ipython().system('date')


# In[3]:

get_ipython().system('pwd')


# In[4]:

get_ipython().magic('mkdir deneme')
get_ipython().magic('cd deneme')


# In[5]:

get_ipython().magic('cd ..')


# In[6]:

get_ipython().run_cell_magic('writefile', 'hello.py', 'print "Hello everyone!"')


# In[7]:

get_ipython().magic('run hello.py')


# In[8]:

get_ipython().magic('lsmagic')


# In[9]:

get_ipython().run_cell_magic('html', '', '\n<html>\n<body>\n\n<h1 style="color:DodgerBlue;"> This is 32th Python Hour 🐍💻 </h1>\n\n</body>\n</html>')


# In[10]:

import sys
print sys.version_info


# # description and setup
# 
# You need to enter your API credentials to the key list. https://apps.twitter.com/

# In[11]:

import json #responses will be in JSON format
import oauth2 

Consumer_Key = "K6Rk0fGEFB6gtKYuzaBnTMT9T"
Consumer_Secret = "F3NryI1Nw3fXGawq2DYQBeowmzsmxWba4wLgPKyUucudwJ35mY"

Access_Token = "455237513-3NZrkuCQMOldFRhuC53r8WoKDY7T5MxjqYyWgzHb"
Access_Token_Secret = "YnCFJANRcEnPPqUM8TcbVbHS0lQX3KjQnKdPU3XdG2sTy"

consumer = oauth2.Consumer(key=Consumer_Key, secret=Consumer_Secret)
access_token = oauth2.Token(key=Access_Token, secret=Access_Token_Secret)
client = oauth2.Client(consumer, access_token)


# # get profile
# 
# Enter the screen_name of the user whose profile will be fetched. https://dev.twitter.com/rest/reference/get/users/lookup

# In[12]:

import urllib, cStringIO
from PIL import Image
from IPython import display


# In[13]:

usernames = ["gvanrossum", "pyistanbul", "hipolabs"]


# In[14]:

request = "https://api.twitter.com/1.1/users/lookup.json?screen_name=" + ",".join(usernames)
response,data = client.request(request)


# # show profile information
# 
# You can specify which info you need to the info list. 

# In[15]:

def show_user_profile(user_data):
    info = ["profile_image_url", "name", "location", "created_at", "lang", "protected", "description", "time_zone", "followers_count"]
    for i in info:
        if i == "profile_image_url":
            URL = user_data[i]
            file = cStringIO.StringIO(urllib.urlopen(URL).read())
            img = Image.open(file)
            display.display(img)
        else:
            print i , ":" ,  user_data[i]


# In[16]:

for ix,user in enumerate(usernames):
    show_user_profile(json.loads(data)[ix])


# # get followers and show a full profile
# 
# https://dev.twitter.com/rest/reference/get/followers/ids <br>
# https://dev.twitter.com/rest/reference/get/users/show

# In[17]:

username = "pyistanbul"


# In[18]:

request = "https://api.twitter.com/1.1/followers/ids.json?screen_name=" + username + "&skip_status=true&include_user_entities=false&count=100"
response,data = client.request(request)


# In[19]:

response


# In[20]:

followers = json.loads(data)["ids"]
followers


# In[21]:

request = '{url1}{user_id}'.format(url1="https://api.twitter.com/1.1/users/show.json?user_id=", user_id=followers[1])
response, data = client.request(request)
shows = json.loads(data)
shows


# # get data from MongoDB

# In[22]:

from pymongo import MongoClient


# In[23]:

mongo_client = MongoClient('localhost', 27017)
db = mongo_client['graph_db']
graph_profile_collection = db.graph_profile_collection 


# # create table using pandas

# In[24]:

import pandas as pd


# In[25]:

profile = pd.DataFrame(list(graph_profile_collection.find()))
profile


# # get locations from Italy

# In[26]:

profile['location'].value_counts().to_frame()


# In[27]:

f = open('cities_italy.txt', 'r')
location_list = f.read().split('\n')

profile_cities_list = []

for location in location_list:
    profile_cities_list.append(profile[profile.location == location])
profile_cities = pd.concat(profile_cities_list)

profile_cities


# # make clickable hyperlink to Twitter Url

# In[28]:

from IPython.display import HTML


# In[29]:

twitter_url = profile_cities['screen_name'].apply(lambda x: '<a href="http://twitter.com/{0}">{0}</a>'.format(x))
profile_cities = profile_cities.assign(twitter_url=twitter_url.values)
html_format = HTML(profile_cities.to_html(escape=False))
html_format


# # list elements according to some columns

# In[30]:

profile.sort_values(by=['followers_count'], ascending=False)[['screen_name', 'followers_count']].tail(10)


# In[31]:

profile['location'] = profile['location'].str.lower()
profile[['screen_name','location']].head(10)


# # map location (string) to (city, country)

# In[32]:

location_list = []
locations = profile['location'].str.lower()

for index in range(len(profile['location'].head(10))):
    location_list.append(locations[index].encode('utf-8'))

print 'Location:', location_list[4]


# In[33]:

from geopy.geocoders import Nominatim


# In[34]:

nom = Nominatim(timeout=None)
geo1 = nom.geocode(location_list[4], addressdetails=True)

print 'City: ', geo1.raw['address']['city'].encode('utf-8'), '\n', 'Country: ', geo1.raw['address']['country'].encode('utf-8')


# In[ ]:



