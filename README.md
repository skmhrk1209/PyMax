# PyMax
Embedding a python interpreter in Max external objects using pybind11

```python:add.py
import typing

def add(x: float, y: float) -> typing.Tuple[float]:
    return (x + y,)
```

<img width="367" alt="01" src="https://user-images.githubusercontent.com/29158616/90301773-d0794000-dedc-11ea-9944-bbc554e0cd56.png">
