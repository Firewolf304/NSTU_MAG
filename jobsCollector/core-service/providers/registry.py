# system
from typing import Dict, List

# local
from consts.ProviderType import ProviderType
from .implementations.base import baseProvider
from .implementations.headhuner import HeadHunterProvider

class ProviderRegistry:
    _providers: Dict[ProviderType, baseProvider] = {}
    
    @classmethod
    def register(cls, provider: baseProvider):
        cls._providers[provider.providerType] = provider
        
    @classmethod
    def get(cls, name: ProviderType) -> baseProvider:
        if name not in cls._providers:
            raise ValueError(f"Provider {name} not registered")
        return cls._providers[name]
    
    @classmethod
    def list_all(cls) -> List[ProviderType]:
        return list(cls._providers.keys())