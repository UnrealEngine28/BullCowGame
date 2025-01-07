// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Hiddenwordlist.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();



    Isograms = GetValidWords(Words);

    SetUpGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ProcessGuess(Input);
   
}

void UBullCowCartridge::SetUpGame() 
{
    PrintLine(TEXT("Welcome Player this is Bulls and Cows"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() -1)];
    Lives = HiddenWord.Len()*2;
    bGameOver = false;

    PrintLine(TEXT("You have %i lives remaining"), Lives);
    PrintLine(TEXT("Guess the %i Letter word"), HiddenWord.Len());
    PrintLine(TEXT("Type in your guess \nAnd then press enter to continue"));
    //PrintLine(TEXT("The HiddenWord is %s"), *HiddenWord); //debug line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;

    PrintLine(TEXT("\npress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) 
{
    if (bGameOver)
    {
        ClearScreen();
        SetUpGame();
        return;
    }

    // process guess 1
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("Congratulations you win"));
        EndGame();
        return;
    }

    // process guess 2
    if (Guess.Len() != HiddenWord.Len()) 
    {   
        ClearScreen();
        PrintLine(TEXT("The hiddenword is %i Characters long"), HiddenWord.Len());
        PrintLine(TEXT("Try guessing again\n*You have %i lives remaining*"), Lives);
        return;
    }

    //process guess 3
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again !"));
        return;
    }

    PrintLine(TEXT("You have lost a life"));
    --Lives;
    
    if (Lives<=0) 
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The hidden word was: %s "), *HiddenWord);
        EndGame();
        return;
    }
    
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Try guessing again \nyou have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{

    int32 Index = 0;
    int32 Comparison = Index + 1;

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;

}

TArray<FString> UBullCowCartridge :: GetValidWords (const TArray<FString>& WordList) const 
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
        ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) 
        {
            Count.Bulls++;
            continue;
        }
        for ( int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
};