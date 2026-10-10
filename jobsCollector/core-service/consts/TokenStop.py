from enum import Enum

class TokenState(Enum):
    null: int = -1
    rate_limit: int = 0
    banned: int = 1
    other: int = 2