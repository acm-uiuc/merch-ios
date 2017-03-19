//
//  ViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/9/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    // MARK: - Outlets
    @IBOutlet var circleViews: [MUICircleView]!
    @IBOutlet var keypadButtons: [MUIButton]!

    @IBOutlet weak var clearButton: UIButton!
    @IBOutlet weak var deleteButton: UIButton!

    @IBOutlet weak var errorLabel: MUIHidingLabel!

    // MARK: - Variables
    var passcode = "" {
        /*  Precondition: the passcode should never be modified by more than one
                character at a time except when being cleared
         */
        didSet {
            let count = passcode.characters.count
            switch count {
            case 0:
                clearButton.isEnabled = false
                deleteButton.isEnabled = false
                let _ = keypadButtons.map { $0.isEnabled = true }
                
            case 1..<8:
                clearButton.isEnabled = true
                deleteButton.isEnabled = true

            case 8:
                clearButton.isEnabled = false
                deleteButton.isEnabled = false
                let _ = keypadButtons.map { $0.isEnabled = false }
                // Attempt Login
            default:
                assertionFailure("passcode length should never exceed 8 characters")
            }
            
        }
    }

    // MARK: - UIViewController
    override func viewDidLoad() {
        super.viewDidLoad()
        passcode = ""
        circleViews = circleViews.sorted { return $0.tag < $1.tag }
    }
    
    // MARK: - Actions
    @IBAction func didSelectKeypadButton(_ button: UIButton) {
        guard let number = button.titleLabel?.text else {
            assertionFailure("Incorrect storyboard setup, please check that every keypad button has a number")
            return
        }
        passcode += number
        circleViews[passcode.characters.count - 1].filled = true
    }
    
    @IBAction func didSelectClear(_ sender: UIButton) {
        passcode = ""
        let _ = circleViews.map { $0.filled = false }
    }

    @IBAction func didSelectDelete(_ sender: UIButton) {
        passcode.remove(at: passcode.index(before: passcode.endIndex))
        circleViews[passcode.characters.count].filled = false
    }
    
    // MARK: - Login
    
    
    
}

