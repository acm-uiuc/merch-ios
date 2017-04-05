//
//  LoginViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/9/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit
import GrootSwift

class LoginViewController: UIViewController {
    
    // MARK: - Outlets
    @IBOutlet weak var passcodeBubblesContainerView: UIView!
    @IBOutlet var passcodeBubbles: [UIImageView]!
    @IBOutlet var keypadButtons: [UIButton]!

    @IBOutlet weak var deleteButton: UIButton!

    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var detailLabel: UILabel!

    // MARK: - Variables
    var passcode = "" {
        /*  Precondition: the passcode will never be modified by more than one
                character at a time except when being cleared
         */
        didSet {
            let count = passcode.characters.count
            switch count {
            case 0:
                deleteButton.isEnabled = false
                
                titleLabel.alpha = 1.0
                let _ = keypadButtons.map { $0.isEnabled = true }
                let _ = passcodeBubbles.map { $0.alpha = 1.0 }

            case 1..<passcodeBubbles.count:
                deleteButton.isEnabled = true
                
                change(detailLabel, toText: nil, animated: true)
                
            case passcodeBubbles.count:
                deleteButton.isEnabled = false
                
                titleLabel.alpha = 0.3
                let _ = keypadButtons.map { $0.isEnabled = false }
                let _ = passcodeBubbles.map { $0.alpha = 0.3 }
                
                change(detailLabel, toColor: UIConstants.Colors.secondary, animated: false)
                change(detailLabel, toText: "Authenticating", animated: true)
                
                attemptLogin()
            default:
                assertionFailure("passcode length should never exceed `passcodeBubbles.count` number of characters")
            }
        }
    }
    
    // MARK: - UIViewController
    override func viewDidLoad() {
        super.viewDidLoad()
        // ensures all the passcodeBubbles are in order, the tags in storboard
        //  must be assending for this to work
        passcodeBubbles = passcodeBubbles.sorted { return $0.tag < $1.tag }
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        // reset all UI to default values and clear UserModel
        passcode = ""
        
        let _ = passcodeBubbles.map { $0.isHighlighted = false }
        change(detailLabel, toText: nil, animated: false)
        UserModel.shared = nil
    }
    
    // MARK: - Actions
    @IBAction func didSelectKeypadButton(_ button: UIButton) {
        guard passcode.characters.count < passcodeBubbles.count else { return }
        guard let number = button.titleLabel?.text else {
            assertionFailure("Incorrect storyboard setup, please check that every keypad button has a number")
            return
        }
        passcode += number
        passcodeBubbles[passcode.characters.count - 1].isHighlighted = true
    }


    @IBAction func didSelectDelete(_ sender: Any?) {
        guard passcode.characters.count > 0 else { return }
        passcode.remove(at: passcode.index(before: passcode.endIndex))
        passcodeBubbles[passcode.characters.count].isHighlighted = false
    }
    
    // MARK: - Detail Label
    func change(_ label: UILabel, toColor newColor: UIColor, animated: Bool) {
        guard label.textColor != newColor else { return }
        
        if animated {
            UIView.animate(withDuration: 0.25) {
                label.textColor = newColor
            }
        } else {
            label.textColor = newColor
        }
    }
    
    func change(_ label: UILabel, toText newText: String?, animated: Bool) {
        guard label.text != newText else { return }
        
        if animated {
            UIView.transition(with: label, duration: 0.25, options: .transitionCrossDissolve, animations: {
                label.text = newText
            }, completion: nil)
        } else {
            label.text = newText
        }
    }
    
    // MARK: - Login
    func attemptLogin() {
        MerchService.getUser(passcode: passcode, success: { (json) in
            if let model = json["data"] as? [String: Any] {
                UserModel.load(json: model)
            }
            
            if UserModel.shared != nil {
                DispatchQueue.main.async {
                    self.loginSucceeded()
                }
            } else {
                DispatchQueue.main.async {
                    self.loginFailed(error: "Internal error; unable parse returned data.")
                }
            }
            
        }) { (error) in
            DispatchQueue.main.async {
                self.loginFailed(error: error)
            }
        }.perform(withAuthorization: nil)
    }
    
    func loginSucceeded() {
        performSegue(withIdentifier: "PresentItems", sender: nil)
    }
    
    func loginFailed(error: String) {
        change(detailLabel, toColor: UIConstants.Colors.error, animated: true)
        change(detailLabel, toText: error, animated: true)
        
        passcodeBubblesContainerView.transform = CGAffineTransform(translationX: 80, y: 0)
        UIView.animate(withDuration: 0.4, delay: 0.0, usingSpringWithDamping: 0.2, initialSpringVelocity: 0.8, options: .curveEaseInOut, animations: {
            self.passcodeBubblesContainerView.transform = .identity
        }) { (_) in
            self.passcode = ""
            let _ = self.passcodeBubbles.map { $0.isHighlighted = false }
        }
    }
}

