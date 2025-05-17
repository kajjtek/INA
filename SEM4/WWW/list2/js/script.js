const words = ["KOT", "SAMOCHÓD", "DRZEWO", "PROGRAMOWANIE", "LAPTOP", "ROWER", "PŁYTA", "ŻABA", "ŚLIMAK", "ŁÓDŹ"];  
const alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŹŻ";

let selectedWord = "";
let guessedLetters = [];
let clickedLetters = [];
let wrongGuesses = 0;
const maxWrongGuesses = 6;

const wordContainer = document.getElementById("wordContainer");
const lettersContainer = document.getElementById("lettersContainer");
const canvas = document.getElementById("hangman");
const ctx = canvas.getContext("2d");
const newGameBtn = document.getElementById("newGameBtn");
const cancelBtn = document.getElementById("cancelBtn");

newGameBtn.addEventListener('click', startNewGame);
cancelBtn.addEventListener('click', cancelGame);

function drawHangman() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.strokeStyle = "black";
    ctx.lineWidth = 2;

    ctx.beginPath(); ctx.moveTo(10, 190); ctx.lineTo(190, 190); ctx.stroke();
    ctx.beginPath(); ctx.moveTo(50, 190); ctx.lineTo(50, 20);
    ctx.lineTo(120, 20); ctx.lineTo(120, 40); ctx.stroke();

    if (wrongGuesses > 0) { ctx.beginPath(); ctx.arc(120, 60, 20, 0, Math.PI * 2); ctx.stroke(); }
    if (wrongGuesses > 1) { ctx.beginPath(); ctx.moveTo(120, 80); ctx.lineTo(120, 130); ctx.stroke(); }
    if (wrongGuesses > 2) { ctx.beginPath(); ctx.moveTo(120, 90); ctx.lineTo(90, 110); ctx.stroke(); }
    if (wrongGuesses > 3) { ctx.beginPath(); ctx.moveTo(120, 90); ctx.lineTo(150, 110); ctx.stroke(); }
    if (wrongGuesses > 4) { ctx.beginPath(); ctx.moveTo(120, 130); ctx.lineTo(90, 170); ctx.stroke(); }
    if (wrongGuesses > 5) { ctx.beginPath(); ctx.moveTo(120, 130); ctx.lineTo(150, 170); ctx.stroke(); }
}

function displayWord() {
    wordContainer.innerHTML = selectedWord
        .split("")
        .map(l => guessedLetters.includes(l) ? l : '_')
        .join(' ');
}

function createLetters() {
    lettersContainer.innerHTML = '';
    for (let l of alphabet) {
        const btn = document.createElement('div');
        btn.classList.add('letter');
        btn.textContent = l;
        if (clickedLetters.includes(l)) btn.classList.add('clicked');
        btn.addEventListener('click', () => guessLetter(l, btn));
        lettersContainer.appendChild(btn);
    }
}

function guessLetter(letter, btn) {
    if (btn.classList.contains('clicked')) return;
    btn.classList.add('clicked');

    clickedLetters.push(letter);

    if (selectedWord.includes(letter)) {
        guessedLetters.push(letter);
        displayWord();
        saveState();
        if (!wordContainer.textContent.includes('_')) {
            alert('Wygrałeś!');
            clearState();
        }
    } else {
        wrongGuesses++;
        drawHangman();
        saveState();
        if (wrongGuesses >= maxWrongGuesses) {
            alert('Przegrałeś! Słowo to: ' + selectedWord);
            clearState();
        }
    }
}

function startNewGame() {
    selectedWord = words[Math.floor(Math.random() * words.length)];
    guessedLetters = [];
    clickedLetters = [];
    wrongGuesses = 0;
    drawHangman();
    createLetters();
    displayWord();
    saveState();
}

function cancelGame() {
    if (confirm('Czy na pewno chcesz anulować grę?')) {
        clearState();
        startNewGame();
    }
}

function saveState() {
    localStorage.setItem('hangmanGameState', JSON.stringify({ selectedWord, guessedLetters, wrongGuesses, clickedLetters }));
}

function loadState() {
    const state = JSON.parse(localStorage.getItem('hangmanGameState'));
    if (state) {
        selectedWord = state.selectedWord;
        guessedLetters = state.guessedLetters;
        wrongGuesses = state.wrongGuesses;
        clickedLetters = state.clickedLetters || [];
        drawHangman();
        createLetters();
        displayWord();
    } else {
        startNewGame();
    }
}

function clearState() {
    localStorage.removeItem('hangmanGameState');
    clickedLetters = [];
}

loadState();