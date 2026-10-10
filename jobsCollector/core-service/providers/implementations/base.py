# system
from abc import ABC
import asyncio
from typing import Optional, AsyncIterable, AsyncGenerator

# local
from consts.ProviderType import ProviderType
from models.local.token import Token
from tools.connManager import AIOManagerConnection

class baseProvider (ABC):
    """based provider"""
    
    providerType : ProviderType
    current_token : int = 0
    
    _connManager : AIOManagerConnection = None    
    
    
    def __init__(self, provider : ProviderType, token : list[str]):
        self.providerType = provider
        self.tokens: list[Token] = [
            Token(token=i, provider=self.providerType) for i in token
        ]
        self._lock = asyncio.Lock()
        
    
    async def get_available_token(self) -> Optional[Token]:
        async with self._lock:
            available = [i for i in enumerate(self.tokens) if i[1].is_available()]
            
            if not available:
                return None
            
            # TODO: check round-robin method
            # select first available token
            token = available[0] 
            self.current_token = token[0]
            return token[1]
    
    @classmethod
    async def fetch_jobs_list(
        self, 
        query: str, 
        token: str,
        filters: Dict = None
    ) -> List[Dict]:
        pass
    
    @classmethod
    async def fetch_job_details(
        self, 
        job_id: str, 
        token: str
    ) -> Dict:
        pass
    
    
    
    