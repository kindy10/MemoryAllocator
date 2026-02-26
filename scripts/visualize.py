import json
import plotly.graph_objects as go
import webbrowser
import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def draw_heap(json_file):
    with open(json_file, 'r') as f:
        data = json.load(f)

    fig, ax = plt.subplots(figsize=(12, 2))
    current_x = 0

    for block in data:
        # Couleur : Vert si libre, Rouge si occupé
        color = '#2ecc71' if block['free'] else '#e74c3c'

        # Dessiner le bloc (Payload + Header estimé)
        width = block['size'] + 24
        rect = patches.Rectangle((current_x, 0), width, 1,
                                 linewidth=1, edgecolor='black', facecolor=color)
        ax.add_patch(rect)

        # Ajouter le texte de la taille au milieu
        if width > 100:  # On n'affiche le texte que si le bloc est assez large
            plt.text(current_x + width / 2, 0.5, f"{block['size']} bytes",
                     ha='center', va='center', color='white', fontweight='bold')

        current_x += width

    plt.xlim(0, 4096)  # Taille totale de ton pool
    plt.ylim(0, 1)
    ax.set_yticks([])
    plt.title("Memory allocator visualization")
    plt.tight_layout()
    plt.show()
def  visualize_memory(json_file):
    if not os.path.exists(json_file):
        return
    with open(json_file, 'r') as f:
        data = json.load(f)
    fig = go.Figure()

    for i, bloc in enumerate(data):
        # Style of parametres
        status = "Free" if bloc['free'] else "Allocated"
        color = "green" if bloc['free'] else "red"
        fig.add_trace(go.Bar(
            name=f"Bloc {i}",
            x=[bloc['size']],
            y=['Memory'],
            orientation="h",
            marker=dict(color=color, line=dict(color='rgb(0,0,0)', width=1)),
            customdata=[[bloc['address'], bloc['size'], status]],
            hovertemplate=(
                    "<b>Bloc " + str(i) + "</b><br>" +
                    "Statut: %{customdata[2]}<br>" +
                    "Taille: %{customdata[1]} bytes<br>" +
                    "Adresse: %{customdata[0]}<br>" +
                    "<extra></extra>"
            )
        ))
        fig.update_layout(
            title="Memory Allocation ",
            barmode="stack",
            showlegend=False,
            height=200,
            template="plotly_dark",
            xaxis=dict(title="Bytes (Total 4096)", range=[0, 4096]),
            yaxis=dict(visible=False)
        )

        #Sauvegarder dans le meme dossier que le JSON
        output_path = os.path.join(os.path.dirname(json_file), "Heap.html")
        fig.write_html(output_path)
        print(f"Generated Visualization for Bloc here:{output_path}")

        #Open automatically the browser
        webbrowser.open_new_tab('file://' + os.path.realpath(output_path))


    if __name__ == "__main__":
        #On trouve le dossier parent du script
        BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        #On pointe vers el JSON dans le dossier de build
        path = os.path.join(BASE_DIR,"cmake-build-debug", "heap_data.json")
        draw_heap(path)
        visualize_memory(path)