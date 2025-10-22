import json
import subprocess
import sys
from pathlib import Path
from textual.app import App, ComposeResult
from textual.widgets import Header, Footer, Tree, Static, Input
from textual.containers import Horizontal, Vertical, Container
from textual.widgets import RichLog
from textual.screen import ModalScreen
from rich.syntax import Syntax

VRMLSC = "C:\\Users\\Christian\\sandbox\\repositories\\git\\VrmlScriptCompiler\\build\\Debug\\vrmlsc.exe"  # oder absoluter Pfad falls nötig
#C:\Users\Christian\sandbox\repositories\git\VrmlScriptCompiler\calctest
class SourceView(Static):
    def __init__(self, text: str = "", path: Path | None = None, id: str | None = None):
        super().__init__(id=id)
        self.text = text
        self.path = path

    def render(self):
        code = self.text if self.text else "# No file loaded"
        return Syntax(code, "javascript", theme="monokai", line_numbers=True)

class InputModal(ModalScreen):
    """Modal Screen für Dateipfad-Eingabe"""
    
    CSS = """
    InputModal {
        align: center middle;
    }
    
    #input_container {
        width: 80;
        height: auto;
        background: $panel;
        border: thick $primary;
        padding: 1 2;
    }
    
    #file_input {
        width: 100%;
        margin-bottom: 1;
    }
    """
    
    def compose(self) -> ComposeResult:
        with Vertical(id="input_container"):
            yield Static("Enter file path:", id="input_label")
            yield Input(placeholder="Path to *.js / *.vrmlscript …", id="file_input")

    def on_mount(self):
        self.query_one("#file_input", Input).focus()

    def on_input_submitted(self, event: Input.Submitted) -> None:
        self.dismiss(event.value.strip())

class VrmlUI(App):
    CSS = """
    Screen { 
        layout: vertical; 
    }
    
    #main { 
        layout: horizontal; 
        height: 1fr;
    }
    
    #src { 
        width: 60%; 
        height: 100%; 
        border: solid $primary;
        overflow-x: auto;
        overflow-y: auto;
    }
    
    #ast_container {
        width: 40%;
        height: 100%;
        border: solid $accent;
    }

    #ast {
        width: 100%;
        height: 100%;
    }
  
    #status { 
        height: 6;
        border: solid $success; 
        overflow-y: auto;
    }
    """

    BINDINGS = [
        ("o", "open", "Open"),
        ("p", "parse", "Parse"),
        ("q", "quit", "Quit"),
    ]

    def __init__(self):
        super().__init__()
        self.source = ""
        self.path: Path | None = None
        
    def compose(self) -> ComposeResult:
        yield Header(show_clock=True)
        with Horizontal(id="main"):
            yield SourceView("", None, id="src")
            with Container(id="ast_container"): 
                yield Tree("AST (no file parsed)", id="ast")
        yield RichLog(highlight=True, markup=True, id="status")
        yield Footer()

    def on_mount(self):
        self.query_one("#status", RichLog).write("[cyan]Ready. Press 'o' to open file, 'p' to parse[/cyan]")

    def action_open(self):
        self.push_screen(InputModal(), self.handle_file_input)

    def handle_file_input(self, filepath: str) -> None:
        if not filepath:
            return
        p = Path(filepath)
        status = self.query_one("#status", RichLog)

        if not p.exists():
            status.write(f"[red]✗ File not found:[/red] {p}")
            return
        
        try:
            self.path = p
            self.source = p.read_text(encoding="utf-8", errors="ignore")
            
            # Update Source View
            src_view = self.query_one("#src", SourceView)
            src_view.text = self.source
            src_view.path = self.path
            src_view.refresh()
            
            # Clear AST
            ast_tree = self.query_one("#ast", Tree)
            ast_tree.clear()
            ast_tree.root.label = "AST (not parsed yet)"
            
            status.write(f"[green]Loaded:[/green] {p} ({len(self.source)} chars)")
            
        except Exception as e:
            status.write(f"[red]Error loading file:[/red] {e}")

    def action_parse(self):
        status = self.query_one("#status", RichLog)

        if not self.source:
            status.write("[red]✗ No source loaded. Press 'o' to open a file.[/red]")
            return
        
        status.write("[yellow]Parsing...[/yellow]")
        
        try:
            # Baue Command
            cmd = [VRMLSC, "--ast-json"]
            input_data = None
            
            if self.path:
                cmd.append(str(self.path))
            else:
                cmd.append("-")
                input_data = self.source.encode("utf-8")
            
            # Führe vrmlsc aus
            proc = subprocess.run(
                cmd,
                input=input_data,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                check=False
            )
            
            if proc.returncode != 0:
                error_msg = proc.stderr.decode('utf-8', errors='ignore')
                status.write(f"[red]✗ Parse failed (exit {proc.returncode}):[/red]")
                status.write(f"[red]{error_msg}[/red]")
                
                ast_tree = self.query_one("#ast", Tree)
                ast_tree.clear()
                ast_tree.root.label = "AST (parse error)"
                return
            # Parse JSON
            try:
                data = json.loads(proc.stdout.decode("utf-8"))
                self._fill_ast_tree(data)
                status.write("[green]Parsed successfully[/green]")
            except json.JSONDecodeError as e:
                status.write(f"[red]Invalid JSON output:[/red] {e}")

        except FileNotFoundError:
            status.write(f"[red]Cannot find vrmlsc executable:[/red]")
            status.write(f"[yellow]Path: {VRMLSC}[/yellow]")
            status.write(f"[yellow]Add to PATH or update path in script[/yellow]")
        except Exception as e:
            status.write(f"[red]Unexpected error:[/red] {e}")

    def _fill_ast_tree(self, data):
        def add_node(parent, obj, depth = 0):
            if depth > 50:
                parent.add("[max depth reached]")
                return
            if isinstance(obj, dict):
                label = obj.get("kind", "object")
                node = parent.add(label)

                for k, v in obj.items():
                    if k == "kind": 
                        continue
                    child = node.add(f"[bold]{k}[/bold]")
                    add_node(child, v, depth + 1)
            elif isinstance(obj, list):
                node = parent.add(f"[dim]list[/dim] ({len(obj)} items)")
                for i, item in enumerate(obj):
                    add_node(node, item, depth + 1)
            else:
                parent.add(f"[cyan]{repr(obj)}[/cyan]")

        ast_tree = self.query_one("#ast", Tree)
        ast_tree.clear();

        root = ast_tree.root
        root.label = "AST Root"
        add_node(root, data)
        root.expand_all()

if __name__ == "__main__":
    VrmlUI().run()
