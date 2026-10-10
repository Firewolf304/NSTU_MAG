# local
from enum import Enum

class AuthType(Enum) :
    api_key = 0
    oauth = 1
    oauth2 = 2