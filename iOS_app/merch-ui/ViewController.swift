//
//  ViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/9/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    var passcode = ""
    
//    var valid = true {
//        didSet {
//            vendButton.isEnabled = valid
//        }
//    }
    
    @IBOutlet weak var errorLabel: UILabel!
    
    @IBOutlet var circleViews: [MUICircleView]!
    @IBOutlet var keypadButtons: [MUIButton]!
    
    @IBOutlet weak var vendButton: MUIButton!
    
    let buttonColor = UIColor(red: 90/255, green: 200/255, blue: 219/255, alpha: 1.0)
    let buttonHighlightColor = UIColor(red: 90/255, green: 200/255, blue: 219/255, alpha: 0.1)

    override func viewDidLoad() {
        super.viewDidLoad()
        
        for button in keypadButtons {
            stylize(button: button)
        }
        
//        stylize(button: vendButton)

        for circleView in circleViews {
            stylize(circleView: circleView)
        }
        
        errorLabel.alpha = 0
    }
    
    func stylize(button: MUIButton) {
        button.layer.masksToBounds = true
        button.layer.cornerRadius = 32
        button.layer.borderWidth = 2
        button.layer.borderColor = buttonColor.cgColor
        button.setTitleColor(buttonColor, for: .normal)
        button.defaultBackgroundColor = nil
        button.highlightedBackgroundColor = buttonHighlightColor
    }
    
    func stylize(circleView: MUICircleView) {
        circleView.layer.masksToBounds = true
        circleView.layer.cornerRadius = 8
        circleView.layer.borderWidth = 2
        circleView.layer.borderColor = buttonColor.cgColor
        circleView.filledColor = buttonHighlightColor
        circleView.unfilledColor = nil
    }

    func displayError() {
//        valid = false
        UIView.animate(withDuration: 0.25) {
            self.errorLabel.alpha = 1
        }
    }
    
    func hideError() {
//        valid = true
        UIView.animate(withDuration: 0.25) {
            self.errorLabel.alpha = 0
        }
    }
    
    @IBAction func didSelectKeyPadButton(_ button: MUIButton) {
        
    }

    func validate() {

    }
    
    @IBAction func vend() {

    }
}

