import inspect
import importlib
import typing


def main(module_name, function_name):
    module = importlib.import_module(module_name)
    function = getattr(module, function_name)
    signature = inspect.signature(function)
    assert all(parameter.annotation is not parameter.empty for parameter in signature.parameters.values())
    assert signature.return_annotation is not signature.empty
    input_annotations = [str(parameter.annotation).split("'")[1] for parameter in signature.parameters.values()]
    output_annotations = [str(annotation).split("'")[1] for annotation in signature.return_annotation.__args__]
    return input_annotations, output_annotations
