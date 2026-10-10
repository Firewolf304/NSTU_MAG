# system
import aiohttp
from enum import Enum
from typing import Optional, Callable, Any, Dict
# local
from providers.base import baseProvider
from consts.ConnectionManager import *

class AIOManagerConnection:
    
    protocol : ConenctionProtocol = ConenctionProtocol.HTTP
    host : str = None
    base_path : str = None
    
    _session : Optional[aiohttp.ClientSession] = None
    _provider: Optional[baseProvider] = None
    _custom_authorize : Callable[[AIOManagerConnection, Dict[str, str], Any], Any] = None
    _imortant_data : Dict[str, Any] = None
    
    # TODO: make support for API keys collection, but now use kwargs
    def __init__(self, protocol: ConenctionProtocol = ConenctionProtocol.HTTP, host : str = "", base_path : str = "", provider : baseProvider = None, **kwargs):
        self._session_kwargs = kwargs
        self._provider = provider
        self._custom_authorize = kwargs['custom_authorize']
        
        self.host = host
        self.protocol = protocol
        self.base_path = base_path
    
    async def __aenter__(self) -> AIOManagerConnection:
        """
        Create the ClientSession when entering the async context.
        """
        if self._session is None or self._session.closed:
            self._session = aiohttp.ClientSession(base_url=self.base_url, **self._session_kwargs)
        return self
    async def __aexit__(self, exc_type, exc_value, traceback):
        """
        Close the ClientSession when exiting the async context.
        """
        await self.close()
        
    async def close(self) -> None:
        """
        Close session.
        """
        if self._session and not self._session.closed:
            await self._session.close()
    
    async def get(self, url: str, **kwargs) -> aiohttp.ClientResponse:
        if not self._session:
            raise RuntimeError("Session is not active. Use 'async with AiohttpConnectionManager()' or call '__aenter__'.")
        token = await self.provider.get_available_token()
        headers = kwargs.get('headers', {})
        if not self._custom_authorize:
            headers['Authorization'] = f"Bearer {token.token}"
            kwargs['headers'] = headers
        else:
            # call custom headers for authorize
            headers = self._custom_authorize(self, headers, token, kwargs)
        return await self._session.get(f"{self.protocol}://{self.host}{self.base_path}{url}", **kwargs)
    
    async def post(self, url: str, **kwargs) -> aiohttp.ClientResponse:
        if not self._session:
             raise RuntimeError("Session is not active.")
        token = await self.provider.get_available_token()
        headers = kwargs.get('headers', {})
        if not self._custom_authorize:
            headers['Authorization'] = f"Bearer {token.token}"
            kwargs['headers'] = headers
        else:
            # call custom headers for authorize
            headers = self._custom_authorize(self, headers, token, kwargs)
        return await self._session.get(f"{self.protocol}://{self.host}{self.base_path}{url}", **kwargs)
