# function
def add(a: int, b: int) -> int:
    """single-line docstring #1"""
    return a + b

def mult(a: int, b: int) -> int:
    """ a multi-line
    docstring #2
    """
    return a * b

def suffix(l: str) -> str:
    # add pound symbol to the end
    return f"{l}#"
