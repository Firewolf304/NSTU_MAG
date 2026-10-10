# System
from dataclasses import dataclass, field
from datetime import datetime, timedelta
from typing import Optional

# Local
from consts import ProviderType, TokenStop

@dataclass
class Token:
    token: str
    provider: ProviderType
    
    # limits
    backoff_counter = 5
    
    # rate limits
    requests_made: int = 0
    requests_limit: int = -1
    delta_available_seconds : int = 60 # delta in seconds
    window_start: datetime = field(default_factory=datetime.now)
    
    # backoff
    backoff_until : Optional[datetime] = None
    
    # States
    is_active : bool = True
    reason_stop : TokenStop = TokenStop.null
    last_message : str = None
    stop_count : int = 0
    
    # local check
    @property
    def is_available(self) -> bool:
        if not self.is_active:
            return False
        if self.reason_stop < -1:
            return False
        if datetime.now() - self.window_start > timedelta(seconds=self.delta_available_seconds):
            return False
        
        return self.requests_made < self.requests_limit
    
    def record_error(self, error: str, reason : TokenStop = TokenStop.other) -> Token:
        self.stop_count += 1
        self.last_message = error
        self.reason_stop = reason
        if self.stop_count > self.backoff_counter:
            backoff = min(300, 2 ** self.consecutive_errors) # count in seconds
            self.backoff_until = datetime.now() + timedelta(seconds=backoff)
    
    def record_request(self) -> Token:
        self.requests_made += 1
        self.consecutive_errors = 0
        self.window_start = datetime.now()
        return self
    
    def set_active(self, active = True) -> None:
        self.is_active = active
    
    