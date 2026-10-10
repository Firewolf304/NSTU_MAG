from typing import Dict, Any, Callable
from functools import wraps

GLOBALS: Dict[str, Any] = {}

# TODO: change old slow global vars

def global_function(func: Callable) -> Callable:
    """Global decorator"""
    GLOBALS[func.__name__] = func
    
    @wraps(func)
    def wrapper(*args, **kwargs):
        return func(*args, **kwargs)
    return wrapper

def get_global_func(name: str):
    """Get global func"""
    def decorator(func: Callable) -> Callable:
        GLOBALS[name] = func()
        return func
    return decorator

def get_global(name: str) -> Any:
    """Get global value by name"""
    return GLOBALS.get(name)

def set_global(name: str, value: Any) -> None:
    """Set global value by name"""
    GLOBALS[name] = value 