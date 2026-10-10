from datetime import datetime, timedelta
from typing import Optional

class OAuthToken:
    def __init__(self, access_token: str, refresh_token: Optional[str] = None, expires_in: int = 3600):
        self.access_token = access_token
        self.refresh_token = refresh_token
        self.expires_at = datetime.now() + timedelta(seconds=expires_in)

    def is_expired(self) -> bool:
        return datetime.now() >= self.expires_at

    def update(self, access_token: str, expires_in: int):
        self.access_token = access_token
        self.expires_at = datetime.now() + timedelta(seconds=expires_in)