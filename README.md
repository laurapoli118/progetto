
# **Rete Neurale**
 Si è implementata una rete neurale di Hopfield in C++ con libreria grafica SFML. All'utente sarà richiesto di inserire il numero di pixel con cui vuole visualizzare l'immagine. Potrà poi scegliere fra le sei foto contenute nella cartella "images" ( si consiglia di selezionare 3/4 immagini per una migliore resa della rete); le immagini selezionate saranno utilizzate per allenare la matrice dei pesi secondo la regola di Hebb. Quando si intenderà interrompere la fase di allenamento della matrice si dovrà digitare "stop". Verrà poi richiesto di scegliere una delle immagini su cui è stata allenata la rete che verrà sporcata con una percentuale selezionata dall'utente. La fase di richiamo farà in modo che l'utente veda l'immagine corretta.

## Compilazione Debug
cmake --build build --config Debug
cmake --build build --config Debug --target test

## Compilazione Release
cmake --build build --config Release
cmake --build build --config Release --target test