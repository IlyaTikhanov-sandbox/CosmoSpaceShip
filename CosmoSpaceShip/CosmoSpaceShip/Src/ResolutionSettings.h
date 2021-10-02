#pragma once

struct ResolutionSettings
{
public:

	void calculateResolution(int newWidth, int newHeight)
	{
		widthInPixel = newWidth;
		heightInPixel = newHeight;

		numTilesWidth = widthInPixel % Base == 0 ? widthInPixel / Base : widthInPixel / Base + 1;
		numTilesHeight = heightInPixel % Base == 0 ? heightInPixel / Base : heightInPixel / Base + 1;

		mediumShipScale = heightInPixel / 250;

		ligthShipScale = mediumShipScale / shipSizeDelta;
		heavyShipScale = mediumShipScale * shipSizeDelta;
	}

	void printResolutionInfo()
	{
		std::cout << "Width = " << widthInPixel << std::endl;
		std::cout << "Height = " << heightInPixel << std::endl;
		std::cout << "Num Tiles in Width = " << numTilesWidth << std::endl;
		std::cout << "Num Tiles in Height = " << numTilesHeight << std::endl;
		std::cout << "Medium ship size width = " << mediumShipSide << std::endl;
		std::cout << "Light ship scale " << ligthShipScale << std::endl;
		std::cout << "Medium ship scale " << mediumShipScale << std::endl;
		std::cout << "Heavy ship scale " << heavyShipScale << std::endl;

		//std::cout << "Screen/MediumShip Ratio Width = " << widthInPixel / mediumShipSide << std::endl;
		//std::cout << "Screen/MediumShip Ratio Height = " << heightInPixel / mediumShipSide << std::endl;
	}

	int getWindowWidth() { return widthInPixel; }
	int getWindowHeight() { return heightInPixel; }
	int getWidthInTiles() { return numTilesWidth; }
	int getHeightInTiles() { return numTilesHeight; }
	int getBaseTileSize() { return Base; }
	int getMediumShipScale() { return mediumShipScale; }
	int getHeavyShipScale() { return heavyShipScale; }

private:
	const int Base = 120; //make it changable

	int widthInPixel;
	int heightInPixel;
					    
	int numTilesWidth;
	int numTilesHeight;  
					    
	int mediumShipSide;
	int ligthShipScale;
	int mediumShipScale;
	int heavyShipScale;

	int shipSizeDelta = 2;
};