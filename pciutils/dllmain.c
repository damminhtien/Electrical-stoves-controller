/* Replace "dll.h" with the name of your header */
#include "dll.h"
#include <windows.h>
#include <stdio.h>
#include "lib/pci.h"

DLLIMPORT void HelloWorld()
{
	MessageBox(0,"Hello World from DLL!\n","Hi",MB_ICONINFORMATION);
}

DLLIMPORT void ConnectPCI()
{
	struct pci_access *pacc;
	struct pci_dev *dev;
	unsigned int c;
	char namebuf[1024], *name;
	
	pacc = pci_alloc();		/* Get the pci_access structure */
	/* Set all options you want -- here we stick with the defaults */
	pci_init(pacc);		/* Initialize the PCI library */
	pci_scan_bus(pacc);		/* We want to get the list of devices */
	for (dev=pacc->devices; dev; dev=dev->next)	/* Iterate over all devices */
    {
		pci_fill_info(dev, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);	/* Fill in header info we need */
    	c = pci_read_byte(dev, PCI_INTERRUPT_PIN);				/* Read config register directly */
    	printf("%04x:%02x:%02x.%d vendor=%04x device=%04x class=%04x irq=%d (pin %d) base0=%lx",
	    dev->domain, dev->bus, dev->dev, dev->func, dev->vendor_id, dev->device_id,
	    dev->device_class, dev->irq, c, (long) dev->base_addr[0]);

    	/* Look up and print the full name of the device */
    	name = pci_lookup_name(pacc, namebuf, sizeof(namebuf), PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id);
    	printf(" (%s)\n", name);
    }
	pci_cleanup(pacc);		/* Close everything */
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}
	
	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}
