# system

# local
from .base import *
from consts.ConnectionManager import ConenctionProtocol

class HeadHunterProvider(baseProvider):
    
    connManager : AIOManagerConnection = None
    
    # url
    protocol : ConenctionProtocol = ConenctionProtocol.HTTPS
    host : str = "api.hh.ru"
    
    
    # init class
    def __init__(self, provider : ProviderType, token : list[str], url : str = None):
        """
        Init provider method.
        """
        super().__init__(provider, token)

    
    def authorize(self):
        self.connManager = AIOManagerConnection(
            protocol=self.protocol,
            host=self.host,
            provider=self.providerType,
            custom_authorize = lambda manager, headers, token : headers | {"Authorization": "Bearer token"}
        )
        self.connManager.post("/token", )