#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TStyle.h>

int main() {
    // Nome del file CSV
    std::string filename = "../plugins/laser_corrections.csv";

    // Vettori per memorizzare i dati
    std::vector<double> timeSinceStart;
    std::vector<double> laserCorrections;

    // Apri il file CSV
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore: non è stato possibile aprire il file " << filename << std::endl;
        return 1;
    }

    // Salta l'intestazione
    std::string line;
    std::getline(file, line);

    // Leggi i dati dal file CSV
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string timeStr, correctionStr;

        // Estrai il tempo e la correzione laser
        std::getline(ss, timeStr, ',');
        std::getline(ss, correctionStr, ',');

        // Converti le stringhe in valori double
        double time = std::stod(timeStr);
        double correction = std::stod(correctionStr);

        // Aggiungi i valori ai vettori
        timeSinceStart.push_back(time);
        laserCorrections.push_back(correction);
    }

    file.close();

    // Crea il grafico usando ROOT
    TGraph* graph = new TGraph(timeSinceStart.size(), &timeSinceStart[0], &laserCorrections[0]);

    // Stile del grafico
    graph->SetTitle("Laser Correction vs Time;Time Since Start (s);Laser Correction");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1);
    graph->SetLineWidth(2);

    // Crea un canvas per visualizzare il grafico
    TCanvas* canvas = new TCanvas("canvas", "Laser Correction vs Time", 800, 600);

    // Disegna il grafico
    graph->Draw("APL");

    // Aggiungi una griglia
    canvas->SetGrid();

    // Mostra il canvas
    canvas->Update();
    canvas->Draw();

    // Salva il grafico come immagine
    canvas->SaveAs("laser_correction_vs_time.png");

    // Mantieni aperta la finestra ROOT
    canvas->WaitPrimitive();

    delete graph;
    delete canvas;

    return 0;
}
